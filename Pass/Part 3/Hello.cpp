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
		int outLoopsTotal;
		int exitsTotal;

		//Initialize
		moduleStats(){
			loopsTotal = 0;
			outLoopsTotal = 0;
			exitsTotal = 0;
		}

		//Print all information
		void print(){
			errs() << "Total Loops:" << loopsTotal << '\n' ;
			errs() << "Outermost Loops:" << outLoopsTotal << '\n' ;
			errs() << "Exit CFG:" << exitsTotal << '\n' ;
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
			
			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();

			///////////////////////////DO LOOP STUFF//////////////////////////////////////////////////////////////
			//Number of Loops
			for (LoopInfo::iterator newLoop = LI.begin(); newLoop != LI.end(); newLoop++) {
					fullStats.loopsTotal++;
			}

			/////////////////////////////////DO BASIC BLOCK STUFF/////////////////////////////////////////////////
			//Get list of basic blocks			
			Function::BasicBlockListType &allblocks = F.getBasicBlockList();
			//Go through basic blocks
			for (Function::iterator newBlock = allblocks.begin(); newBlock != allblocks.end(); newBlock++) {

				//Check for if basic block is loop header
				if(LI.isLoopHeader(newBlock)==true){
					//if it is, check depth
					if(LI.getLoopDepth(newBlock)==1){					
						fullStats.outLoopsTotal++;
					}				
				}

				//Check for CFG
				if (LI.getLoopDepth(newBlock)!=0 && newBlock->getTerminator()->getOpcode() == 2){			//if call
					Loop* leave = LI.getLoopFor(newBlock);
					//Go through all successors					
					for (int i = 0; i< (newBlock->getTerminator())->getNumSuccessors();i++){
 						BasicBlock* successorBlock = (newBlock->getTerminator())->getSuccessor(i);
						Loop* enter = LI.getLoopFor(successorBlock);
						//if loop left is diff then enter, increment exit
						if (enter!=leave){
							fullStats.exitsTotal++;
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

