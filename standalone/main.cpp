#include "gfcpu_source.h"
#include "gfpublisher_skel.h"
#include "gfpublisher.h"

using namespace Grafips;

int main(int argc, const char **argv)
{
    CpuSource prov;
    PublisherImpl pub;
    prov.SetMetricSink(&pub);
    int port = 53136;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }
    PublisherSkeleton skel(port, &pub);
    skel.Start();
    prov.Start();

    skel.Join();
    return 0;
}
