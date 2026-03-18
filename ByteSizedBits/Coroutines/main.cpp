#include "AsyncSleeper.h"
#include "AbandonedAsyncSleeper.h"
using namespace std;



void PrintVersions()
{
    cout << "_MSC_VER: " << _MSC_VER << endl;
    cout << "_MSC_FULL_VER: " << _MSC_FULL_VER << endl;
}

int main()
{

    // -------------------------------------------------- //

    PrintVersions();

    // -------------------------------------------------- //

	MakeAndRunAsyncSleeperCoroutine();

    // -------------------------------------------------- //

    Create_AbandonedSleeperCoroutine();
    Start_AbandonedSleeperCoroutine();
    Resume_AbandonedSleeperCoroutine();
    
    // -------------------------------------------------- //

}

