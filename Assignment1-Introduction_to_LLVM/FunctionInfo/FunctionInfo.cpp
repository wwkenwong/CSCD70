#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


namespace {

class FunctionInfo final : public ModulePass
{
public:
	static char ID;

	FunctionInfo() : ModulePass(ID) {}
	virtual ~FunctionInfo() override {}

  	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const override
	{
		AU.setPreservesAll();
	}

	virtual bool runOnModule(Module & M) override
	{
		outs() << "CSCD70 Functions Information Pass" << "\n";
		outs() << "Name # Args # Calls # Blocks # Insts" << "\n";
		// @TODO Please complete this function.

		for (auto iter = M.begin(); iter != M.end(); ++iter) {
			// do some stuff for each function
		    Function &F = *iter;
			// https://llvm.org/doxygen/classllvm_1_1Function.html
			// https://llvm.org/doxygen/classllvm_1_1Value.html
			// https://llvm.org/doxygen/Function_8h_source.html 
			outs() << F.getName() << " ";
			outs() << F.arg_size() << " ";
			outs() << F.getNumUses() << " ";
			outs() << F.getBasicBlockList().size() << " ";
			outs() << F.getInstructionCount() << "\n";
		}
		// It should return true if the module 
		// was modified by the transformation and false otherwise.
		return false;
	}
};

char FunctionInfo::ID = 0;
RegisterPass < FunctionInfo > X (
	"function-info",
	"CSCD70: Functions Information");

}  // namespace anonymous
