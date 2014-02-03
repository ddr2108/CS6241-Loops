#define DEBUG_TYPE "p32"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {

	//Structure to hold stats about module
	struct moduleStats{
		int loopsTotal;

		//Initialize
		moduleStats(){
			loopsTotal = 0;
		}

		//Print all information
		void print(){
			errs() << "Total Loops:" << loopsTotal << '\n' ;
		}

		~moduleStats(){
			print();
		}
	};
	
	struct p32 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p32() : FunctionPass(ID) {}

		//Initialize moduleStats
		moduleStats fullStats;

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int ind;			
			int i,j;
			int *dist; 
			BasicBlock* successorBlock;

			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();


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
						dist[i*ind + j] = 1000000000;
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

			//Perform the algorithm
			for (int x = 0; x<ind ;x++){
				for (int y = 0; y<ind ;y++){
					for (int z = 0; z<ind ;z++){
						if (y!=z){
							if (dist[y*ind + z] > (dist[y*ind + x] + dist[x*ind + z])){ 
								dist[y*ind + z] = dist[y*ind + x] + dist[x*ind + z];
							}
						}
					}
				}
			}


			//Find the loops
			for (i = 0; i<ind; i++){
				for(j =0; j<ind; j++){
					if(dist[i*ind + j]==1){		//look for back edge
						if(dist[j*ind + i]>0 && dist[j*ind + i]!=1000000000){
							fullStats.loopsTotal++;		//Increment number of loops
						}					
					}
				}
			}
		
			free(dist);

			return false; 
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
		  AU.addRequired<LoopInfo>();
		}

	};
}

char p32::ID = 0;
static RegisterPass<p32> X("p32", "Part 3.2", true, true);

