#include <unistd.h>
#include "CivetServer.h"

#include "JobundleHandler.h"

int main(int argc, char *argv[])
{
    const char *options[] = {
	"document_root", ".", "listening_ports", "8080", 0};

    std::vector<std::string> cpp_options;
    for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
        cpp_options.push_back(options[i]);
    }

    JobundleHandler jh;
    CivetServer server(cpp_options);
    server.addHandler("/jobs/templates", jh);

    while (1) {
        sleep(1);
    }

    return 1;
}
