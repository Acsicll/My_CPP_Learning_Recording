#include <iostream>
#include "test_functions.h"
using namespace CanNoTRunInVscode;
int main()
{
  // TestCircularQueSeq();
  // TestCircularQueueSeqInMutiThread();
  // TestThreadSafeQueueHt();
  // TestThreadSafeHash();
  // TestThreadSafeList();
  // TesaLockFreeStack();
  // MultiThreadPush();
  // TesaLockFreeStack();
  // TestHazardPointerStack();
  // TestSingleRefStack();
  // AtomicMemoryOperator::TesatReleastSeq();
  TestRefCountStack();
  std::cout << "Done" << std::endl;
  return 0;
}