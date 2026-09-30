#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"enum.h"

#define HEADER_SIZE 10

#define GREEN  "\033[0;32m"
#define RESET  "\033[0m"

typedef struct mp3reader 
{
    char *mp3_name;
    FILE* mp3_file;

    char* title_name;
    unsigned int title_size;

    char* artist_name;
    unsigned int artist_size;

    unsigned int track_num_size;

    char* album_name;
    unsigned int album_name_size;

    int mp3_year;

    char* genre;

    char *comment;
    unsigned int comment_size;

    /* for editing */

    char* mp3_name_edit;
    FILE* mp3_file_edit;   // actual file name

    unsigned int frame_id_content_size;

    char *req_tag_frame;

    char *new_content;

    FILE *temp_mp3_fptr_edit;

}reader;

void printing_invalid_arg(void);        //fun declaration for printing invalid arguments

void printing_opt(void);                //fun declaration for printing options

status reading_mp3_file(reader *str,char* argv[]);      //fun declaration for opening mp3 file

status reading_file_cont(reader* str);

status reading_artist_name(reader* str);                //fun declaration for the reading the artist name form the mp3 file

status reading_track_num(reader* str);                   //fun declaration for thr reading the track number

status reading_album(reader* str);                       // fun decalration for the reading the album from the mp3 file

status reading_year(reader* str);

status reading_genre(reader* str);                       //fun declaration for the reading the genre of the mp3 file

status reading_composer(reader* str);                     //fun declaration for the reading the composer from the mp3 file

status check_ID3(reader* str,char* argv[]);               //fun declaration for the validating the id3 is it present in the mp3 file

status mp3_check_edit(char* argv[]);                      //fun declaration for the validating the mp3 file or not

status check_opt_edit(reader* str,char**argv);            //fun declaration for the checking the argc[2]

status finding_tag_frame_edit(char* argv[],reader* str);               //fun declaration for the finding the tag frame in the mp3 file

status copy_to_temp(reader** str);                //fun declaration for the coping the content from the original mp3 file to temp file

