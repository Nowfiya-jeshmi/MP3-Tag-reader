#include"header.h"

void printing_invalid_arg(void)                                       //fun definition for printing invalid arguments
{
    printf("-------------------------------------------------------------\n");
    
    printf("ERROR : ./a.out : INVALID ARGUMENTS\n");
    
    printf("USAGE :\n");
    
    printf("\tTo view please pass like : ./a.out -v mp3filename\n");
    
    printf("\tTo edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    
    printf("\tTo get help pass like : ./a.out --help\n");
    
    printf("-------------------------------------------------------------\n");
}

void printing_opt(void)                                                //fun definition for printing help options                 
{
    
    printf("-------------------------------------------------------------HELP MENU-------------------------------------------------------------\n\n");
    
    printf("1. -v -> to view mp3 file contents\n");
    
    printf("2. -e -> to edit mp3 file contents\n");
    
    printf("\t2.1. -t -> to edit song title\n");
    
    printf("\t2.2. -a -> to edit artist name\n");
    
    printf("\t2.3. -A -> to edit album name\n");
    
    printf("\t2.4. -y -> to edit year\n");
    
    printf("\t2.5. -m -> to edit content\n");
    
    printf("\t2.6. -c -> to edit comment\n");
    
    printf("\n-------------------------------------------------------------------------------------------------------------------------------\n");

}

status reading_mp3_file(reader *str,char* argv[])
{
    str -> mp3_name = argv[2];

    str -> mp3_file = fopen(str -> mp3_name,"r");

    char buffer_id3[4];

    fread(buffer_id3,3,1,str -> mp3_file);

    buffer_id3[4] = '\0';

    if(strcmp(buffer_id3,"ID3")==0)
    {
        printf("The given mp3 file is in ID3 format\n\n");
    }

    char buffer_ver[1];

    fread(buffer_ver,1,1,str -> mp3_file);

    printf("The given mp3 file version is in ID3v2.%d format\n\n",buffer_ver[0]);

    fseek(str -> mp3_file,6,SEEK_SET);

    unsigned char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    unsigned long int size = (buffer_size[0] << 21) | (buffer_size[1] << 14) | (buffer_size[2] << 7) | (buffer_size[3]) ;    //it is 7 bit model ( sync-safe ) only header

    printf("The mp3 file metadata size's is %ld\n\n",size);

    return e_success;
}

status reading_file_cont(reader* str)                                 //fun definition for the reading the file contents
{

    char buffer[5];

    fread(buffer,4,1,str -> mp3_file);

    buffer[4] = '\0';

    printf("%s -- ",buffer);                      // TIT2 = title frame

    char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    unsigned int size = (buffer_size[0]) << 24 | (buffer_size[1]) << 16 | (buffer_size[2]) << 8 | (buffer_size[3]) ; // it is a normal 32 bit integer 

    str -> title_size = size;

    fseek(str -> mp3_file,2,SEEK_CUR);

    char buffer_title[size+1];                                  //+1 because the unicode is present in the first byte of each tag's content

    fread(buffer_title,size,1,str -> mp3_file);

    buffer_title[size] = '\0';

    str -> title_name = buffer_title ;

    printf("%s(Title)\n",buffer_title+1);

    return e_success;

}

status reading_artist_name(reader* str)                               //fun declaration for the reading the artist name form the mp3 file
{

    char buffer[4];

    fread(buffer,4,1,str -> mp3_file);

    buffer[4] = '\0';

    printf("%s -- ",buffer);

    char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    str -> artist_size = (buffer_size[0] << 24) | (buffer_size[1] << 16) | (buffer_size[2] << 8) | (buffer_size[3]) ;       //this is the extracting the size from the hexa decimal form and by using the bitwise operator

    // printf("The size of the artist is %u\n",str -> artist_size);

    fseek(str -> mp3_file,2,SEEK_CUR);

    char buffer_artist[str -> artist_size+1];                       //+1 because the unicode is present in the first byte of each tag

    fread(buffer_artist,str -> artist_size,1,str -> mp3_file);

    buffer_artist[str -> artist_size] = '\0';

    str -> artist_name = buffer_artist+1;

    printf("%s (Artist Name)\n",str -> artist_name);

    return e_success;

}

status reading_track_num(reader* str)                                 //fun definition for the reading the track number from the mp3 File
{
    char buffer_tag_frame[4];

    fread(buffer_tag_frame,4,1,str -> mp3_file);

    printf("%s -- ",buffer_tag_frame);

    char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    str -> track_num_size = (buffer_size[0] << 24) | (buffer_size[1] << 16) | (buffer_size[2] << 8) | (buffer_size[3]) ;

    fseek(str -> mp3_file,2,SEEK_CUR);              //skiping the flag

    char buffer[str -> track_num_size+1];

    fread(buffer,str -> track_num_size,1,str -> mp3_file);

    buffer[str -> track_num_size] = '\0';

    printf("%s \n",buffer+1);

    return e_success;
}

status reading_album(reader* str)              // fun defanition for the reading the album from the mp3 file
{
    char buffer_tag_frame[4];

    fread(buffer_tag_frame,4,1,str -> mp3_file);

    buffer_tag_frame[4] = '\0';

    printf("%s -- ",buffer_tag_frame);

    char buffer_alb_size[4];

    fread(buffer_alb_size,4,1,str -> mp3_file);

    str -> album_name_size = (buffer_alb_size[0] << 24) | (buffer_alb_size[1] << 16) | (buffer_alb_size[2] << 8) | (buffer_alb_size[3]) ;

    fseek(str -> mp3_file , 2 ,SEEK_CUR);        //skipping the 2 bytes of flag

    char buffer_album_name[str -> album_name_size +1];    //skip the 1 byte because it is the unicode

    fread(buffer_album_name,str -> album_name_size,1,str -> mp3_file);

    buffer_album_name[str -> album_name_size] = '\0';

    str -> album_name = buffer_album_name + 1;

    printf("%s \n",str -> album_name);

    return e_success;

}

status reading_year(reader* str)
{
    char buffer_tag_frame[4];

    fread(buffer_tag_frame,4,1,str -> mp3_file);

    printf("%s -- ",buffer_tag_frame);

    char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    unsigned size = (buffer_size[0] << 24) | (buffer_size[1] << 16) | (buffer_size[2] << 8) | (buffer_size[3] ) ;

    fseek(str -> mp3_file , 2 , SEEK_CUR);

    char buffer_year[size + 1];

    fread(buffer_year , size , 1 , str -> mp3_file);

    buffer_year[size] = '\0';

    str -> mp3_year = atoi(buffer_year+1);

    printf("%d \n",str -> mp3_year);

    return e_success;
}

status reading_genre(reader* str)                                     //fun definition for the reading the genre of the mp3 file
{
    char buffer[4];

    fread(buffer,4,1,str -> mp3_file);

    buffer[4] = '\0';

    printf("%s -- ",buffer);

    char buffer_size[4];

    fread(buffer_size,4,1,str -> mp3_file);

    unsigned size = (buffer_size[0] << 24) | (buffer_size[1] << 16) | (buffer_size[2] << 8) | (buffer_size[3] ) ;

    fseek(str -> mp3_file , 2 , SEEK_CUR);

    char buffer_genre[size+1];

    fread(buffer_genre,size,1,str -> mp3_file);

    buffer_genre[size] = '\0';

    str -> genre = buffer_genre+1;

    printf("%s \n", str -> genre);

    return e_success;

}



