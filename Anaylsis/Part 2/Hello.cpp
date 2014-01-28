//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "hello"
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
	
	struct Hello : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		Hello() : FunctionPass(ID) {}

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
						if (dist[y*ind + z] > (dist[y*ind + x] + dist[x*ind + z])){ 
							dist[y*ind + z] = dist[y*ind + x] + dist[x*ind + z];
						}
					}
				}
			}


			//Find the loops
			for (i = 0; i<ind; i++){
				for(j =0; j<ind; j++){
					if(dist[i*ind + j]==1){		//look for back edge
						if(dist[j*ind + i]>0){
							fullStats.loopsTotal++;		//Increment number of loops
						}					
					}
				}
			}

			return false; 
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
		  AU.addRequired<LoopInfo>();
		}

	};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Part 3", true, true);

