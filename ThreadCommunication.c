#include <libgen.h>

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#include <sys/time.h>

#include <wait.h>



/*
 
0 -> 1

1 -> (2,3,4,5,6)

6 -> (7,8)


1 -> (2,3,4) SIGUSR1
2 -> (5,6) SIGUSR2

6 -> 7 SIGUSR1

7 -> 8 SIGUSR1

8 -> 1 SIGUSR2
 
*/



char *fname;

int proc_n;

int receivedSig = 0;

int sentSigUSR1 = 0;

int sentSigUSR2 = 0;



void fwritePid(const char *dest, int pid)
{

    FILE *file = fopen(dest, "w");



    if (file == NULL)
    {

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));

        return;

    }



    fprintf(file, "%d", pid);

    fclose(file);

}



int freadPid(const char *src)
{

    struct stat fileInfo;

    int returnCode = lstat(src, &fileInfo);



    if (returnCode == EOF)
    {

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));

        return -1;

    }


    int fileSize = (int) fileInfo.st_size + 1;

    FILE *file = fopen(src, "r");



    if (file == NULL)
    {

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));

        return -1;

    }



    char *pointer;

    char pidStr[fileSize];


    fgets(pidStr, fileSize, file);

    long pid = strtol(pidStr, &pointer, 10);



    if (pid == 0)
    { //|| pid == LONG_MIN || pid == LONG_MAX) {

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));

        return -1;

    }



    fclose(file);


    return (int) pid;

}



long getCurrentTime(void)
{

    struct timeval currentTime;

    gettimeofday(&currentTime, NULL);

    return currentTime.tv_usec % 1000;

}



void sendSIGUSR(int sig, int procNumb, int groupId)
{

    if (sig == SIGUSR1)
    {

        sentSigUSR1++;

        fprintf(stdout,"%d %d %d send SIGUSR1 %ld\n", procNumb, getpid(), getppid(), getCurrentTime());

        fflush(stdout);

        kill(-groupId, SIGUSR1);

    }

    else
    {

        sentSigUSR2++;

        fprintf(stdout,"%d %d %d send SIGUSR2 %ld\n", procNumb, getpid(), getppid(), getCurrentTime());

        fflush(stdout);

        kill(-groupId, SIGUSR2);

    }

}



void receiveSIGUSR1()
{

    fprintf(stdout,"%d %d %d get SIGUSR1 %ld\n", proc_n, getpid(), getppid(), getCurrentTime());

    fflush(stdout);

    receivedSig++;

}



void receiveSIGUSR2()
{

    fprintf(stdout,"%d %d %d get SIGUSR2 %ld\n", proc_n, getpid(), getppid(), getCurrentTime());

    fflush(stdout);

    receivedSig++;

}



void receiveSIGTERM()

{

    fprintf(stdout,"%d %d finish works after %d's signal SIGUSR1 and %d's signal SIGUSR2\n", getpid(), getppid(), sentSigUSR1, sentSigUSR2);

    fflush(stdout);

}



void FirstHandleSIGUSR2()

{

    receiveSIGUSR2();


    int pid_2 = freadPid("second.txt");

    int pid_3 = freadPid("third.txt");

    int pid_4 = freadPid("fourth.txt");

    int pid_5 = freadPid("fifth.txt");

    int pid_6 = freadPid("sixth.txt");



    if (receivedSig == 101)
 
   {

        kill(pid_2, SIGTERM);

        kill(pid_3, SIGTERM);

        kill(pid_4, SIGTERM);

        kill(pid_5, SIGTERM);

        kill(pid_6, SIGTERM);

        waitpid(pid_2, 0, 0);

        waitpid(pid_3, 0, 0);

        waitpid(pid_4, 0, 0);

        waitpid(pid_5, 0, 0);

        waitpid(pid_6, 0, 0);

        exit(0);

    }



    sendSIGUSR(SIGUSR1, proc_n, pid_2);

}



void SecondHandleSIGUSR1()

{

    receiveSIGUSR1();

    int pid_5 = freadPid("fifth.txt");

    sendSIGUSR(SIGUSR2, proc_n, pid_5);

}



void ThirdHandleSIGUSR1()

{

    receiveSIGUSR1();

}



void FourthHandleSIGUSR1()

{

    receiveSIGUSR1();

}



void FifthHandleSIGUSR2()

{

    receiveSIGUSR2();

}



void SixthHandleSIGUSR2()

{

    receiveSIGUSR2();

    int pid_7 = freadPid("seventh.txt");

    sendSIGUSR(SIGUSR1, proc_n, pid_7);

}



void SeventhHandleSIGUSR1()

{

    receiveSIGUSR1();

    int pid_8 = freadPid("eighth.txt");

    sendSIGUSR(SIGUSR1, proc_n, pid_8);

}



void EighthHandleSIGUSR1()

{

    receiveSIGUSR1();

    int pid_1 = freadPid("first.txt");

    sendSIGUSR(SIGUSR2, proc_n, pid_1);

}



void FirstHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void SecondHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void ThirdHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void FourthHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void FifthHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void SixthHandleSIGTERM()
{
 
   receiveSIGTERM();


    int pid_7 = freadPid("seventh.txt");

    int pid_8 = freadPid("eighth.txt");


    kill(pid_7, SIGTERM);

    kill(pid_8, SIGTERM);

    waitpid(pid_7, 0, 0);

    waitpid(pid_8, 0, 0);


    exit(0);

}



void SeventhHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void EighthHandleSIGTERM()
{

    receiveSIGTERM();

    exit(0);

}



void initFirstHandlerSIGUSR2 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = FirstHandleSIGUSR2;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR2);

    action.sa_mask = set;


    sigaction(SIGUSR2, &action, 0);

}



void initSecondHandlerSIGUSR1 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SecondHandleSIGUSR1;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    action.sa_mask = set;


    sigaction(SIGUSR1, &action, 0);

}



void initThirdHandlerSIGUSR1 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = ThirdHandleSIGUSR1;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    action.sa_mask = set;


    sigaction(SIGUSR1, &action, 0);

}



void initFourthHandlerSIGUSR1 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = FourthHandleSIGUSR1;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    action.sa_mask = set;


    sigaction(SIGUSR1, &action, 0);

}



void initFifthHandlerSIGUSR2 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = FifthHandleSIGUSR2;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR2);

    action.sa_mask = set;


    sigaction(SIGUSR2, &action, 0);

}



void initSixthHandlerSIGUSR2 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SixthHandleSIGUSR2;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR2);

    action.sa_mask = set;


    sigaction(SIGUSR2, &action, 0);

}



void initSeventhHandlerSIGUSR1 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SeventhHandleSIGUSR1;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    action.sa_mask = set;


    sigaction(SIGUSR1, &action, 0);

}



void initEighthHandlerSIGUSR1 (int procNumb)
{

    proc_n = procNumb;


    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = EighthHandleSIGUSR1;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGUSR1);

    action.sa_mask = set;


    sigaction(SIGUSR1, &action, 0);

}



void initSecondHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SecondHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}

void initThirdHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = ThirdHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}



void initFourthHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = FourthHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}



void initFifthHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = FifthHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}



void initSixthHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SixthHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);
 
    sigaddset(&set, SIGTERM);

    action.sa_mask = set;

 
    sigaction(SIGTERM, &action, 0);

}



void initSeventhHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = SeventhHandleSIGTERM;


    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}



void initEighthHandlerSIGTERM()
{

    struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = EighthHandleSIGTERM;

    sigset_t set;

    sigemptyset(&set);

    sigaddset(&set, SIGTERM);

    action.sa_mask = set;


    sigaction(SIGTERM, &action, 0);

}



_Noreturn void proc_2();
_Noreturn void proc_3();
_Noreturn void proc_4();

_Noreturn void proc_5();
_Noreturn void proc_6();
_Noreturn void proc_7();
_Noreturn void proc_8();



_Noreturn void proc_1()
{

    fwritePid("first.txt", getpid());

    initFirstHandlerSIGUSR2(1);


    int returnCode = setpgid(getpid(), getpid());



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));


    int pid_2 = fork();



    if (pid_2 == 0) 
    {

        proc_2();

    }
    else
    {

        int pid_3 = fork();



        if (pid_3 == 0)
        {

            proc_3();

        }
        else
	{

            int pid_4 = fork();



            if (pid_4 == 0)
	    {

                proc_4();

            }
	    else
	    {

                int pid_5 = fork();



                if (pid_5 == 0)
		{

                    proc_5();

                }
		else
		{

                    int pid_6 = fork();



                    if (pid_6 == 0)
		    {

                        proc_6();

                    }
  		    else
		    {

                        struct stat fileInfo;


                        while (lstat("second.txt", &fileInfo) == EOF) {}

                        while (lstat("third.txt", &fileInfo) == EOF) {}

                        while (lstat("fourth.txt", &fileInfo) == EOF) {}

                        while (lstat("fifth.txt", &fileInfo) == EOF) {}

                        while (lstat("sixth.txt", &fileInfo) == EOF) {}

                        while (lstat("seventh.txt", &fileInfo) == EOF) {}

                        while (lstat("eighth.txt", &fileInfo) == EOF) {}



                        sendSIGUSR(SIGUSR1, 1, pid_2);

                        sentSigUSR1++;



                        while (1)
			{

                            pause();

                        }

                    }

                }

            }

        }

    }

}



_Noreturn void proc_2()
{

    fwritePid("second.txt", getpid());

    initSecondHandlerSIGUSR1(2);

    initSecondHandlerSIGTERM();


    int returnCode = setpgid(getpid(), getpid());


    
    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    while (1)
    {

        pause();

    }

}



_Noreturn void proc_3()
{

    fwritePid("third.txt", getpid());

    initThirdHandlerSIGUSR1(3);

    initThirdHandlerSIGTERM();


    struct stat fileInfo;


    while (lstat("second.txt", &fileInfo) == EOF) {}


    int secondProcPid = freadPid("second.txt");

    int returnCode = setpgid(getpid(), secondProcPid);



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    while (1)
    {

        pause();

    }

}



_Noreturn void proc_4()
{

    fwritePid("fourth.txt", getpid());

    initFourthHandlerSIGUSR1(4);

    initFourthHandlerSIGTERM();


    struct stat fileInfo;


    while (lstat("second.txt", &fileInfo) == EOF) {}


    int secondProcPid = freadPid("second.txt");

    int returnCode = setpgid(getpid(), secondProcPid);



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    while (1)
    {

        pause();

    }

}



_Noreturn void proc_5()
{

    fwritePid("fifth.txt", getpid());

    initFifthHandlerSIGUSR2(5);

    initFifthHandlerSIGTERM();


    int returnCode = setpgid(getpid(), getpid());



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    while (1)
    {

        pause();

    }

}



_Noreturn void proc_6()
{

    fwritePid("sixth.txt", getpid());

    initSixthHandlerSIGUSR2(6);

    initSixthHandlerSIGTERM();


    struct stat fileInfo;


    while (lstat("fifth.txt", &fileInfo) == EOF) {}


    int fifthProcPid = freadPid("fifth.txt");

    int returnCode = setpgid(getpid(), fifthProcPid);



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    int pid_7 = fork();



    if (pid_7 == 0)
    {

        proc_7();

    }
    else
    {

        int pid_8 = fork();



        if (pid_8 == 0)
	{

            proc_8();

        }
        else
        {

            while (1)
	    {

                pause();
            }

        }

    }

}



_Noreturn void proc_7()
{

    fwritePid("seventh.txt", getpid());

    initSeventhHandlerSIGUSR1(7);

    initSeventhHandlerSIGTERM();


    int returnCode = setpgid(getpid(), getpid());



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    while (1)
    {

        pause();

    }

}



_Noreturn void proc_8()
{

    int returnCode = setpgid(getpid(), getpid());



    if (returnCode == -1)

        fprintf(stderr, "%d: %s: %s\n", getpid(), fname, strerror(errno));



    int mainProcessPid = freadPid("root.txt");


    char psTreeCommand[50] = {0};

    sprintf(psTreeCommand, "pstree -p %d", mainProcessPid);

    system(psTreeCommand);


    initEighthHandlerSIGUSR1(8);

    initEighthHandlerSIGTERM();

    fwritePid("eighth.txt", getpid());



    while (1)
    {

        pause();

    }

}



int main(int argc, char *argv[])
{

    fname = basename(argv[0]);



    if (fname == NULL)
    {

        fprintf(stderr,"%d: Problem with executable file %s", getpid(), argv[0]);

        return 1;

    }



    fwritePid("root.txt", getpid());


    int pid_1 = fork();



    if (pid_1 == 0)
    {

        proc_1();

    }

    else
    {

        waitpid(pid_1, 0, 0);

    }



    return 0;

}