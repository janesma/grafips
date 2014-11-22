#include "gfcpu_provider.h"
#include "gfpublisher_remote.h"
#include "gfpublisher.h"

using namespace Grafips;

int main()
{
    CpuProvider prov;
    PublisherImpl pub;
    prov.setPublisher(&pub);
    PublisherSkeleton skel(53136, &pub);
    skel.Start();
    prov.start();

    skel.Join();
    return 0;
}
