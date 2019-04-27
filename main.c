
//enum Error {ARGS, SYS_CALL};//TODO:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define END_FILE_STATUS 0
#define ARG_NUMBERS 3
#define ERR_EXIT 4
#define FIRST_FILE_INDEX 1
#define SECOND_FILE_INDEX 2

#define NON_COUNT_STRING " \n"
#define SPACE ' '
#define NEWLINE '\n'
#define TAB '\t'
#define EQUAL 1
#define SIMILAR 3
#define DIFFERENT_FILES 2
#define ERR_FLAG -1

#define ERROR_SYS "Error in system call!\n"
#define ERROR_NUM_ARGS "Wrong number of arguments!\n"

/**
 * Handling with the Error situations adm checkes if
 * it needed to close the corrents files
 * @param file1
 * @param file2
 */
void errorExecution(int* file1, int* file2){//TODO:ENUM
    if(*file1!=END_FILE_STATUS)
        close(*file1);
    if (*file2 !=END_FILE_STATUS)
        close(*file2);
    write(2, ERROR_SYS, strlen(ERROR_SYS));
    exit(ERR_EXIT);
}

/**
 * function that close all readers.
 * @param file1 - the first file
 * @param file2 - the second file
 */
void closeReaders(int * file1, int * file2){
    close(*file1);
    close(*file2);
}

/**
 * check if there is space or new line.
 * @param buffer - the char buffer
 * @return 1 - if there is. 0- if not.
 */
int checkIfSpace(char buffer){
    if (buffer == SPACE || buffer == NEWLINE
        || buffer==TAB)
        return 1;
    return 0;
}

/**
 * converts the char to lower case
 * @param c1 - the first char
 * @param c2 - the second char
 */
void  lower(char * c1, char *c2 ) {
    if (*c1 <= 90 && *c1>= 65 ) {
        *c1 += 32;
    }
    if (*c2 <= 90 && *c2>= 65 ) {
        *c2 += 32;
    }
}

/**
 * check if the files are equal.
 * @param firstFile - the first file
 * @param secondFile - the second file
 * @return 1 - if equal, 0- if not.
 */
int checkEqual(const char *firstFile, const char *secondFile, int lenght) {
    for (int i=0; i<lenght; i++){
        if(firstFile[i]!=secondFile[i]){
            return 0;
        }
    }
    return 1;
}

/**
 * check if the files are similar to each other
 * @param firstFile - the first file
 * @param secondFile - the second file
 * @return 1 - if similar, 0 - if not.
 */

int checkSimilar(char *firstFile, char *secondFile, int lenght) {
    int offset1=0, offset2=0;
    int result = 0;
    for (int i=0; i<lenght;i++){
        if(firstFile[i+offset1]!=secondFile[i+offset2]){
            lower(&firstFile[i+offset1], &secondFile[i+offset2]);
            if(firstFile[i+offset1]!=secondFile[i+offset2]){
                //lower(buffer2, buffer1);
                if(checkIfSpace(firstFile[i+offset1])) {
                    offset1++;
                    i--;
                    result = 0;
                    continue;
                }
                if(checkIfSpace(secondFile[i+offset2])) {
                    offset2++;
                    i--;
                    result = 0;
                    continue;
                }
            }
        }
        result = 1;
    }
    return result;
}


int CheckFileTotalSize(char *filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) == -1){
        return -1;//TODO:error enum
    }
   return ((int)fileStat.st_size);
}

int validLetterCheck(char letter, const char *non_countble, size_t len){
    for (int i=0; i<len; i++){
        if(letter == non_countble[i]){
            return 1;
        }
    }
    return 0;
}

int letterNumberCheck(char *file_contant, int file_size){
    int letter_num = 0;
    for(int i = 0; i < file_size; i++){
        if(!validLetterCheck(file_contant[i], NON_COUNT_STRING, strlen(NON_COUNT_STRING))){
            letter_num += 1;
        }
    }
    return letter_num;
}

int checkLongestFile(int lenght1, int lenght2){
    if (lenght1>lenght2){
        return lenght1;
    }
    return lenght2;
}

/**
 * call the functions that check if the file is same or equal.
 * @param argv - the array of args
 * @return the corrent number.
 */
int compareFiles(char **argv){
    //files descriptors
    int file1_fd, file2_fd;

    int file1_size, file2_size;
    int result_file1, result_file2;
    char *file1 = argv[FIRST_FILE_INDEX];
    char *file2 = argv[SECOND_FILE_INDEX];

    file1_fd = open(file1, O_RDONLY);
    file2_fd = open(file2, O_RDONLY);
    if (file1_fd == -1 || file2_fd == -1) {
        write(2, ERROR_SYS, strlen(ERROR_SYS));
        //errorExecution(END_FILE_STATUS,END_FILE_STATUS);
    }
    file1_size = CheckFileTotalSize(file1);
    file2_size = CheckFileTotalSize(file2);
    char file1_contant[file1_size];
    char file2_contant[file2_size];
    result_file1 = read(file1_fd, file1_contant, file1_size);
    result_file2 = read(file2_fd, file2_contant, file2_size);
    if (result_file1 == ERR_FLAG || result_file2 == ERR_FLAG) {
        errorExecution(&file1_fd, &file2_fd);
    }
    file1_size = letterNumberCheck(file1_contant,file1_size);
    file2_size = letterNumberCheck(file2_contant,file2_size);
    closeReaders(&file1_fd,&file2_fd);
    if (file1_size!=file2_size){
        return DIFFERENT_FILES;
    } else {
        int max = checkLongestFile(file1_size,file2_size);
        if (checkEqual(file1_contant, file2_contant, max)) {
            return EQUAL;
        } else if (checkSimilar(file1_contant, file2_contant, max)) {
            return SIMILAR;
        } else {
            return DIFFERENT_FILES;
        }
    }
}

///**
// * main function
// * @param argc - number of arguments
// * @param argv - array of args
// * @return if the file are equal, same or not.
// */TODO:
int main(int argc, char *argv[]) {
    int fileComparetionResult;
    if (argc != ARG_NUMBERS) {
        write(2, ERROR_NUM_ARGS, strlen(ERROR_NUM_ARGS));
        //errorExecution(END_FILE_STATUS,END_FILE_STATUS);TODO:make enum
    }
    fileComparetionResult = compareFiles(argv);
    return fileComparetionResult;
}