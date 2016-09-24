#include "JobundleHandler.h"
#include "ziplib/ZipFile.h"
#include "PJDL/PVCPparse.h"
#include <unistd.h>
#include <fcntl.h> //tmeporary
#include <string.h>
#include <istream>
#include <iostream>
#include <sys/stat.h>

const char *TEMPDIR = "/home/szopen/test/WWW/civetweb-POVOCOP/povocop/tmp";

bool JobundleHandler::unzip( const char *filename )
{
}

int jobundle_field_found( const char * key,const char *filename, char *path, size_t pathlen, void *user_data)
{
    //TODO check if it is really a zip file
    //TODO different handling if this is pure jpdl
    //printf("KEY %s FILENAME %s PATH %s\n", key, filename, path);
    if ( strcmp(key,"file[archive]") == 0 ) {
        snprintf( path, pathlen, "%s/%s",TEMPDIR,filename);
        //strncpy( path, TEMPDIR , pathlen);
        //printf("PREPARING %s for %s\n", path, filename);
        return FORM_FIELD_STORAGE_STORE;
    }
    return FORM_FIELD_STORAGE_SKIP;
}


int jobundle_field_get( const char *key, const char *value, size_t valuelen, void *user_data)
{
    std::cout << value << std::endl;
    return 0;
}

int jobundle_field_stored( const char *path, long long file_size, void *user_data)
{
    //printf("STORED %s\n", path);
    ZipArchive::Ptr archive = ZipFile::Open(path);

    int noe = archive->GetEntriesCount();
    //printf("N.of.E %d \n",noe);

    std::string outPath("jobs/templates/");
    std::string base(basename( path ));
    
    int pos = base.rfind(".zip");
    outPath += base.substr(0,pos) + "/";

    //TODO: remove if outPath already exists
    mkdir( outPath.c_str(), 0766 );
        std::cout << "Extracting " << outPath << std::endl;
    
    for (int i=0;i<noe;i++) {
        auto entry = archive->GetEntry(i);

        std::string name = entry->GetFullName();
        std::string out = outPath + name;
        if (entry->IsDirectory()) {
	//    std::cout << "Creating directory " << out << std::endl;
            mkdir( out.c_str(), 0766 );
            continue;
        }
     //   std::cout << "Extracting " << out << std::endl;
        ZipFile::ExtractFile( path, name, out );
    }
    remove( path );
    // handle JPDL
    std::string main_pjdl = outPath+"main.pjdl";
    int fd = open( main_pjdl.c_str(), O_RDONLY);
    if (fd < 0 ){
        std::cerr << "No main.jpdl file found in jobundle zip archive " << base  << std::endl;
        return -1;
    }

    int src_size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    char *str = new char[src_size+1];
    int n = read( fd, str, src_size);
    if (n<0) {
        std::cerr << "Error reading file: " << main_pjdl << std::endl;
        std::cerr << "I will exit now." << std::endl;
        exit(0);
    }
    str[src_size]=0;
    PVCP_parse(str);
    //delete str;

 return 0;
}

bool JobundleHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

    
        struct mg_form_data_handler fdh = {jobundle_field_found, jobundle_field_get, jobundle_field_stored, 0};
       const struct mg_request_info *req_info = mg_get_request_info(conn);
	long long rlen, wlen;
	long long nlen = 0;
	long long tlen = req_info->content_length;
	char buf[1025]={0};
        char *membuf = 0;


    mg_printf(conn,
	      "HTTP/1.1 200 OK\r\nContent-Type: "
	      "text/html\r\nConnection: close\r\n\r\n");
    mg_printf(conn, "<html><body>\r\n");
      mg_printf(conn,
		  "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>\n",
		  req_info->request_method,
		  req_info->uri,
		  req_info->http_version);
	mg_printf(conn, "<p>Content Length: %li</p>\n", (long)tlen);
	printf("Content Length: %li\n", (long)tlen);
	mg_printf(conn, "<pre>\n");
printf("req_info->localuri %s req_info->quesry_String %s request_uri\n", req_info->local_uri, req_info->query_string, req_info->request_uri);


        mg_handle_form_request( conn, &fdh );
        return true;


        membuf = new char[ tlen ];
        int x = 0;
	while (nlen < tlen) {
       // while(1) {
		rlen = tlen>_chunksize?_chunksize:tlen; // tlen - nlen;
		/*if (rlen > sizeof(buf)) {
			rlen = sizeof(buf);
		}*/
		//printf("START!\n");
		int ard = mg_read(conn, &(membuf[x]), (size_t)rlen);
                x += ard;
		//printf("[%d]\n", rlen);
		if (rlen <= 0) {
			break;
		}
                //printf("x=%d ard=%d nlen=%d ",x,ard,nlen);
                //printf("%s\n", &(membuf[x]));
		//wlen = mg_write(conn, buf, (size_t)rlen);
		//printf("%s", buf);
		//nlen += wlen;
                nlen += ard;
	}
        int i =0;
        int res=1;

  //       int fd = open("dupa", O_RDWR | O_CREAT | O_TRUNC, 0600);
  //      write( fd, membuf, tlen);
  //      close(fd);
        
printf("\nOK, skonczyłem\n");

	mg_printf(conn, "\n</pre>\n");



	mg_printf(conn, "</body></html>\r\n");
        delete membuf;
	return true;


}
