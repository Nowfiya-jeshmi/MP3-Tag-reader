#include"header.h"

int main(int argc,char* argv[])                             //CLA for reading the requirements
{

    if(argv[1] == NULL)
    {
        printing_invalid_arg();                              //fun call for printing invalid arguments
        return 0;
    }

    else if(argv[2] == NULL)
    {
        printing_invalid_arg();
        return 0;
    }

    else if(strcmp(argv[1],"--help")==0)
    {
        printing_opt();
    }

    if(strcmp(argv[1],"-v")==0)                          //checking the options read
    {

        reader str;                                      //structure variable declaring

        status open;
            
        open = reading_mp3_file(&str,argv);                          //fun call for the reading the file

        if(open == e_success)
        {

            status open_file;
            open_file = reading_file_cont(&str);

            if(open_file == e_success)
            {

                status artist_name;
                artist_name = reading_artist_name(&str);                     //fun call for the reading the artist name from the mp3 file 

                if(artist_name == e_success)
                {

                    status track_fun;
                    track_fun = reading_track_num(&str);                       //fun call for the reading track number

                    if(track_fun == e_success)
                    {

                        status album_fun;
                        album_fun = reading_album(&str);                       // fun call for the reading the album from the mp3 file

                        if(album_fun == e_success)
                        {

                            status year_fun;
                            year_fun = reading_year(&str);

                            if(year_fun == e_success)
                            {

                                status genre_fun;
                                genre_fun = reading_genre(&str);                //fun call for the reading the genre of the mp3 file

                                if(genre_fun == e_success)
                                {

                                

                                    rewind(str.mp3_file);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if(strcmp(argv[1],"-e") == 0)                  //edit option 
    {
        if(argc <= 4)
            {
                printing_invalid_arg();
                return 0;
            }

            reader str;
            status check_id3;                            //validating the id3 is it present in the mp3 file
            check_id3 = check_ID3(&str,argv);

            if(check_id3 == e_success)
            {
                status check_opt;
                check_opt = check_opt_edit(&str,argv);

                if(check_opt == e_success)
                {
                    // printf("This is the tag frame is going to change %s\n",str.req_tag_frame);
                

                status finding_tag_frame;
                finding_tag_frame = finding_tag_frame_edit(argv,&str);

                if(finding_tag_frame == e_success)
                {
                    printf(GREEN"******Changes are done*****\n"RESET);
                    
                }

                }
                else
                {
                    printing_invalid_arg();
                }

            }
            else
            {
                printing_invalid_arg();
            }
    }
}
