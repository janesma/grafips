#include "gfcpu_provider.h"
#include "gfpublisher_skel.h"
#include "gfpublisher.h"

using namespace Grafips;

int main(int argc, const char **argv)
{
    CpuProvider prov;
    PublisherImpl pub;
    prov.setPublisher(&pub);
    int port = 53136;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }
    PublisherSkeleton skel(port, &pub);
    skel.Start();
    prov.start();

    skel.Join();
    return 0;
}
