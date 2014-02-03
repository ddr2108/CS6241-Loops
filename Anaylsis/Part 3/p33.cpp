#define DEBUG_TYPE "p33"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"

using namespace llvm;

namespace {
	
	struct p33 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p33() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int ind;			
			int i,j;
			int *dist; 
			
			//stack
			int stackDom[1000];
			int stackInd = 0;			

			errs() << F.getName() << '\n';

      		PostDominatorTree& PDT = getAnalysis<PostDominatorTree>();

			BasicBlock* successorBlock;

			//Get list of basic blocks			
			Function::BasicBlockListType &allblocks = F.getBasicBlockList();
			//Go through basic blocks
			ind = 0;			
			for (Function::iterator newBlock = allblocks.begin(); newBlock != allblocks.end(); newBlock++) {
				ind++;
			}

			//Allocate array
			dist = (int*) malloc(ind*ind*sizeof(int*));
			for (i = 0; i<ind; i++){
				for(j =0; j<ind; j++){
					if (i==j){
						dist[i*ind + j] = 0;
					}else{
						dist[i*ind + j] = 2147483645;
					}
				}
			}
			//Fill array will all with distance 1;
			i = 0;
			for (Function::const_iterator firstBlock = allblocks.begin(); firstBlock != allblocks.end(); firstBlock++) {
				for (int k = 0; k < (firstBlock->getTerminator())->getNumSuccessors();k++){
					successorBlock = (firstBlock->getTerminator())->getSuccessor(k);	//Successor
					j =0;
					for (Function::const_iterator secondBlock = allblocks.begin(); secondBlock != allblocks.end(); secondBlock++) {	
						//Check if block is right
						if (successorBlock == secondBlock){
							dist[i*ind + j] = 1;
						}
						j++;
					}
				}
				i++;
			}


			//post dominators
			Function::const_iterator thirdBlock = allblocks.begin();
			Function::const_iterator fourthBlock;
			for (int x = 0; x<ind; x++){
				stackInd = 0;
				stackDom[stackInd++] = x;	//add to stack				

				while (stackInd > 0){

					int stackTail = stackDom[--stackInd];		//get the top of stack
					fourthBlock = allblocks.begin();
					for (int y = 0; y<ind; y++){
						if (dist[y*ind + stackTail] == 1 && PDT.dominates(thirdBlock, fourthBlock)){		//if post dominates, add to stack
							stackDom[stackInd++] = y;
						}else if (dist[y*ind + stackTail] == 1){			//otherwise is the control dependecy
							errs() << x << " control dependent on "<< y << '\n';
						}
						
						fourthBlock++;	//get i
					}
				}
				thirdBlock++;		//get the block j
			}


			free(dist);

			return false; 
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      		AU.addRequired<PostDominatorTree>();
		}

	};
}

char p33::ID = 0;
static RegisterPass<p33> X("p33", "Part 3.3", true, true);

