#include <sys/time.h>

using namespace std;

class Sample
{
public:
	Sample() { gettimeofday(&timestamp,0); }
	~Sample() {}

protected:	
	struct timeval timestamp;
};