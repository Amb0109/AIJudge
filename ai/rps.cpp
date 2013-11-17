#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

const char* RPS[] = {
	"Rock",
	"Paper",
	"Scissors"
};

int main()
{
	while(true)
	{
		scanf("%*s");
		puts(RPS[rand()%3]);
		fflush(stdout);
	}
}