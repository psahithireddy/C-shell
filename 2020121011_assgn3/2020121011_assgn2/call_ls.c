#include "headers.h"
#include "call_ls.h"                                 
char pathshell[1024];
void call_ls(char input[])
{
  
    struct passwd *pw; 
    struct group   *grp;
    struct tm      *tm;
    char  outstr[256];   
    int dircount=0;
    char alldir[10][1024]={{}};
    char *token=NULL;
    int count=0;
    int onlydir=0;
    char *argv[1024]={};
    token=strtok(input," ");
    while(token!=NULL)
    {        
       argv[count]=token;
       token=strtok(NULL," ");  
       count++;   
    }                    
    int code=0;   //code is 1 if no flag , is 2 if -l, 3 if -a ,4 if both 
     DIR *dir ;
     struct dirent *d;
     char this_dir[1024]={};	 
     for(int i=0;i<count;i++)
     {
         	
             // printf(" %d is count \n",count);
                 if(count==1 ||strcmp(argv[i],".")==0)
			    {	
                   getcwd(alldir[0],999);
                    code =1;
                    
                }
                else if(strcmp(argv[i],"..")==0)
                {
                    getcwd(alldir[0],999);
                    strcat(alldir[0],"/..");
                    code=1;
                    
                }
                else if(strcmp(argv[i],"~")==0)
                {
                   strcpy(alldir[0],pathshell);
                    code=1;
                }  
                else if(strcmp(argv[i],"-l")==0)
                {
                    if(code==0)
                    {
                       getcwd(alldir[0],999);       // if ls -l is given
                        code =2;
                    }
                    else if(code==3)
                     {   code=4;}
                    else 
                        code=2; // in all other cases dir would have been specified by somebody { . , .. ,~}
                }
                else if(strcmp(argv[i],"-a")==0)
                {
                    if(code==0)
                    {
                        getcwd(alldir[0],999);       // if ls -a is given
                        code=3;
                    }
                    else if(code==2)
                     {  code=4;}
                    else
                        code =3; // in all other cases dir would have been specified by somebody { . , .. ,~}
                }
                else if(strcmp(argv[i],"-al")==0 || strcmp(argv[i],"-la")==0 || strcmp(argv[i],"-l-a")==0 ||strcmp(argv[i],"-a-l")==0)
                {
                    if(code==0)
                        getcwd(alldir[0],999);       // if ls -al is given
                    code =4; // in all other cases dir would have been specified by somebody { . , .. ,~}
                }       
                else if(!(strcmp(argv[i],"ls")==0))
                {
                    strcpy(alldir[dircount],argv[i]);
                    dircount++;
                    if(code ==0)
                    code=1;   
                    onlydir=1;                                 
                   
                }    
                       
         } 
                // till this you will get which dir and which flag to apply
               
				    // universal
                 if(onlydir==0)
                    dircount=1;
                 for(int i=0;i<dircount;i++)
                 {   
                    if(dircount>1)
                    {
                    	char interme[20]={};
                        sprintf(interme,"%s : \n",alldir[i]);
                        write(1,interme,strlen(interme));  
                    }
                    strcpy(this_dir,alldir[i]);	
                    dir = opendir(this_dir);
                    if(dir==NULL)
                    {
                        
                        char temp[1024]="/";
                        getcwd(alldir[i],999); 
                        strcat(temp,argv[i]);            //when dir name is mentioned, store names of dir to be executed
                        strcat(alldir[i],temp);
                        strcpy(this_dir,alldir[i]);
                        dir = opendir(this_dir);
                        if(dir==NULL)
                        {
                             perror("dir");
                             return EXIT_FAILURE;
                        }
                            
                    }
                   if(code==1)
                    {
				        while((d=readdir(dir)) != NULL)
				        {
					       
                             if( !(strcmp(d->d_name,".") == 0 || strcmp(d->d_name,"..") == 0) ) 
						     {
                                char interme[20]={};
                                sprintf(interme,"%s\t",d->d_name);
                                write(1,interme,strlen(interme));  
						      }  			            
			            }
				        closedir(dir);
				        write(1,"\n",1);
                    }
                    if(code==3)
                    {
                        while((d=readdir(dir)) != NULL)
				        {
					        char interme[20]={};
                            sprintf(interme,"%s\t",d->d_name);
                            write(1,interme,strlen(interme));  									            
			            }
				        closedir(dir);
				        write(1,"\n",1);
                    }
                    if(code==2)
                    {
                        struct stat sb;
                        while((d=readdir(dir)) != NULL)
				        {
					       
                             if( !(strcmp(d->d_name,".") == 0 || strcmp(d->d_name,"..") == 0) ) 
						     {
                               //s printf("%s\t",d->d_name);
							    stat(d->d_name, &sb);
                                // file type and  permissions using mode in stat
                                if(S_ISDIR(sb.st_mode)) write(1,"d",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IRUSR) write(1,"r",1);  else write(1,"-",1);
                                if(sb.st_mode & S_IWUSR)  write(1,"w",1);  else write(1,"-",1);	
                                if(sb.st_mode & S_IXUSR)  write(1,"x",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IRGRP)  write(1,"r",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IWGRP)  write(1,"w",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IXGRP)  write(1,"x",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IROTH)  write(1,"r",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IWOTH)  write(1,"w",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IXOTH)  write(1,"x",1);  else write(1,"-",1);
					           
					             char interm1[20]={};
					             sprintf(interm1," %d " ,sb.st_nlink);
					             write(1,interm1,strlen(interm1));   // no of hardlinks
                                // pass uid get passwd and user /grp options
                                if((pw=getpwuid(sb.st_uid))!=NULL)
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8.8s", pw->pw_name);   //username
                                        write(1,interme,strlen(interme));    
                                    }    
                                else
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8d", sb.st_uid);     //if it fails print id 
                                        write(1,interme,strlen(interme));
                                    }		
                                if((grp=getgrgid(sb.st_gid))!=NULL)		
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8.8s", grp->gr_name);  //group name
                                        write(1,interme,strlen(interme));
                                     }   
                                else
                                	{
                                	     char interme[30]={};
                                        sprintf(interme," %-8d", sb.st_gid);    //if it fails print id 
                                        write(1,interme,strlen(interme));
                                     } 
                                char interme[20]={};            
                                sprintf(interme," %7jd ", sb.st_size);	   // to get in propersize  using j
                                write(1,interme,strlen(interme));    
                                //pass m_time and get it locally
                                tm = localtime(&sb.st_mtime);
                                 if (tm == NULL) 
                                 {
									   perror("localtime");
									  // exit(EXIT_FAILURE);
								  }

								  if (strftime(outstr, sizeof(outstr),nl_langinfo( D_T_FMT ) , tm) == 0) 
								  {
									   fprintf(stderr, "strftime returned 0");
									  // exit(EXIT_FAILURE);
								  }

								   write(1, outstr,strlen(outstr));
								   //exit(EXIT_SUCCESS);	 
                                //printf("%s ",dates);
                                char interm[5]={};
                                sprintf(interm," %d ",d->d_type);        
                                write(1,interm,strlen(interm));
								write(1,d->d_name,strlen(d->d_name));   
								write(1,"\n",1);               
                              }                         
			            }
				        closedir(dir);
				        write(1,"\n",1);
				        }
				        if(code==4)
                    {
                        struct stat sb;
                        while((d=readdir(dir)) != NULL)
				        {
                               //s printf("%s\t",d->d_name);
							    stat(d->d_name, &sb);
                                // file type and  permissions using mode in stat
                                if(S_ISDIR(sb.st_mode)) write(1,"d",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IRUSR) write(1,"r",1);  else write(1,"-",1);
                                if(sb.st_mode & S_IWUSR)  write(1,"w",1);  else write(1,"-",1);	
                                if(sb.st_mode & S_IXUSR)  write(1,"x",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IRGRP)  write(1,"r",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IWGRP)  write(1,"w",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IXGRP)  write(1,"x",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IROTH)  write(1,"r",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IWOTH)  write(1,"w",1);  else write(1,"-",1);
					            if(sb.st_mode & S_IXOTH)  write(1,"x",1);  else write(1,"-",1);
					            char interm1[20]={};
					             sprintf(interm1," %d " ,sb.st_nlink);
					             write(1,interm1,strlen(interm1));   // no of hardlinks
                                // pass uid get passwd and user /grp options
                                if((pw=getpwuid(sb.st_uid))!=NULL)
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8.8s", pw->pw_name);   //username
                                        write(1,interme,strlen(interme));    
                                    }    
                                else
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8d", sb.st_uid);     //if it fails print id 
                                        write(1,interme,strlen(interme));
                                    }		
                                if((grp=getgrgid(sb.st_gid))!=NULL)		
                                	{
                                        char interme[30]={};
                                        sprintf(interme," %-8.8s", grp->gr_name);  //group name
                                        write(1,interme,strlen(interme));
                                     }   
                                else
                                	{
                                	     char interme[30]={};
                                        sprintf(interme," %-8d", sb.st_gid);    //if it fails print id 
                                        write(1,interme,strlen(interme));
                                     } 
                                char interme[20]={};            
                                sprintf(interme," %7jd ", sb.st_size);	   // to get in propersize  using j
                                write(1,interme,strlen(interme));    
                                //pass m_time and get it locally
                                tm = localtime(&sb.st_mtime);
                                 if (tm == NULL) 
                                 {
									   perror("localtime");
									  // exit(EXIT_FAILURE);
								  }

								  if (strftime(outstr, sizeof(outstr),nl_langinfo( D_T_FMT ) , tm) == 0) 
								  {
									   fprintf(stderr, "strftime returned 0");
									  // exit(EXIT_FAILURE);
								  }

								   write(1, outstr,strlen(outstr));
								   //exit(EXIT_SUCCESS);	 
                                //printf("%s ",dates);
                                char interm[5]={};
                                sprintf(interm," %d ",d->d_type);        
                                write(1,interm,strlen(interm));
								write(1,d->d_name,strlen(d->d_name));                  
                                write(1,"\n",1);       
			            }
				        closedir(dir);
				        write(1,"\n",1);
				     }
                  }
		    }

    
