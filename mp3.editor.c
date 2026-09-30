#include"header.h"

status check_ID3(reader* str , char* argv[])                       
{

    status mp3_check;

    mp3_check = mp3_check_edit(argv);

    if(mp3_check == e_success)

    {
        
        str -> mp3_name_edit = argv[4];

        str -> mp3_file_edit = fopen(str -> mp3_name_edit,"rb");

        if(str -> mp3_file_edit == NULL)
            {
                return e_failure;
            }

        char buffer_id3[4];

        fread(buffer_id3,3,1,str -> mp3_file_edit);

        buffer_id3[3] = '\0';

        rewind(str -> mp3_file_edit);

        if(strcmp(buffer_id3,"ID3")==0)
            {
                printf("The given mp3 file is in ID3 format\n");
            }

        else
            {
                return e_failure;
            }

        return e_success;

    }

    else
    {
        return e_failure;
    }

}

status mp3_check_edit(char* argv[])                        //fun definition for validating the mp3 file or not 
{
    char* dot = strrchr(argv[4] , '.');                    //extracting the . from the string 

    if(dot == NULL)
    {
        return e_failure;
    }

    else if(strcmp(dot , ".mp3")==0)                            //string compare the .mp3 and from dot till the end of the string
    {
        return e_success;
    }

    else
    {
        return e_failure;
    }

}

status check_opt_edit(reader* str,char**argv)            //fun definition for the checking the argc[2] and storing the new content
{

    if(strcmp(argv[2],"-t")==0)
    {
        str -> req_tag_frame = "TIT2";
        str->new_content = argv[3];
        return e_success;
    }

    else if(strcmp(argv[2],"-a")==0)
    {
        str -> req_tag_frame = "TPE1";
        str->new_content = argv[3];
        return e_success;
    }

    else if(strcmp(argv[2],"-A")==0)
    {
        str -> req_tag_frame = "TALB";
        str->new_content = argv[3];
        return e_success;
    }

    else if(strcmp(argv[2],"-y")==0)
    {
        str -> req_tag_frame = "TDRC";
        str->new_content = argv[3];
        return e_success;
    }

    else if(strcmp(argv[2],"-m")==0)
    {
        str -> req_tag_frame = "TCON";
        str->new_content = argv[3];
        return e_success;
    }

    else if(strcmp(argv[2],"-c")==0)
    {
        str -> req_tag_frame = "WOAR";
        str->new_content = argv[3];
        return e_success;
    }

    return e_failure;
}


status finding_tag_frame_edit(char* argv[],reader* str)                           //fun definition for the finding the tag frame in the mp3 file and for all process
{

    str->temp_mp3_fptr_edit = fopen("temp.mp3","wb");     //opening the temp file

        if(str->temp_mp3_fptr_edit == NULL)
        {
            return e_failure;
        }

    char header[10];                              //copying the header to the temp

    fread(header,10,1,str->mp3_file_edit);

    fwrite(header,10,1,str->temp_mp3_fptr_edit);

    while(1)                                     //loop for finding the tag frame 
        {
            char frame_id[5]; //tag frame

            if(fread(frame_id,4,1,str->mp3_file_edit) != 1)
                break;

            frame_id[4]='\0';

            unsigned char size_buffer[4];   //size    

            fread(size_buffer,4,1,str->mp3_file_edit);

            unsigned int size =(size_buffer[0] << 24) |(size_buffer[1] << 16) |(size_buffer[2] << 8 ) |(size_buffer[3]);
            
            char flags[2];     //flag

            fread(flags,2,1,str->mp3_file_edit);

            if(strcmp(frame_id,str->req_tag_frame)==0)       //comparing the requested tag frame and received tag frame
                {
                    fwrite(frame_id,4,1,str->temp_mp3_fptr_edit);

                    unsigned int new_size = strlen(str->new_content) + 1;   //length of the new content 

                    unsigned char new_size_buf[4];       //converting to bytes
                    
                    new_size_buf[0]=(new_size>>24)&0xFF;
                    
                    new_size_buf[1]=(new_size>>16)&0xFF;

                    new_size_buf[2]=(new_size>>8)&0xFF;

                    new_size_buf[3]=new_size&0xFF;

                    fwrite(new_size_buf,4,1,str->temp_mp3_fptr_edit);    //write size into the temp file

                    fwrite(flags,2,1,str->temp_mp3_fptr_edit);      //write flag to into the temp file

                    char encoding = 0;      //the first byte should be 0 because it is in the most of the IDEv2

                    fwrite(&encoding,1,1,str->temp_mp3_fptr_edit);

                    fwrite(str->new_content, strlen(str->new_content), 1, str->temp_mp3_fptr_edit);       //writing the new content into the temp file

                    fseek(str->mp3_file_edit, size, SEEK_CUR);      //skiping the old contents from the original mp3 file to continue with the next frame

                    continue;

                }

                /* next non targeted frames */

                fwrite(frame_id, 4, 1, str->temp_mp3_fptr_edit);

                fwrite(size_buffer, 4, 1, str->temp_mp3_fptr_edit);

                fwrite(flags, 2, 1 ,str-> temp_mp3_fptr_edit);

                char *buffer = malloc(size);
                
                fread(buffer, size, 1, str->mp3_file_edit);    //copying the contents from original file to temp file

                fwrite(buffer, size, 1, str->temp_mp3_fptr_edit);

                free(buffer);

        }

        char ch;    //copying the remaining datas from the original file to temp file

        while(fread(&ch,1,1,str->mp3_file_edit))
        {
            fwrite(&ch,1,1,str->temp_mp3_fptr_edit);
        }

        fclose(str->mp3_file_edit);
        fclose(str->temp_mp3_fptr_edit);

        remove(str->mp3_name_edit);      //removing the temp file name and renaming it into as original file name

        rename("temp.mp3",str->mp3_name_edit);

        return e_success;

}




