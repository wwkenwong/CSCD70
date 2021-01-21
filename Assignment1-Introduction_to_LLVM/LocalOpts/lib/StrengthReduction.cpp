#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;


namespace {

int64_t return_power(int64_t n) 
{ 
    if (n == 0) 
        return -1; 
    int64_t ctr =0; 
    while (n != 1) 
    { 
        if (n%2 != 0) 
            return -1; 
        ctr+=1;
        n = n/2; 
    } 
    return ctr; 
} 


class StrengthReduction final : public FunctionPass {
 public:
  static char ID;

  StrengthReduction() : FunctionPass(ID) {}
  virtual ~StrengthReduction() override {}

  /*! @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  virtual bool runOnFunction(Function &F) override {
    // Basic Block 
		outs() << "CSCD70 Functions Information Pass" << "\n";
    for (auto &BB:F){
      outs() << "Loop BB" << "\n";
      std::vector<Instruction*> erase_list ;
      for (auto &Inst:BB){
        outs() << "Loop Inst " << "\n";
        if(Inst.getNumOperands()==2){
            outs() << "numOperands == 2  " << "\n";
            bool edited = false ; 
            Value *operand1 = Inst.getOperand(0);
            Value *operand2 = Inst.getOperand(1);
            //Try to get the values 
            // Prevent integer overflow 
            int64_t val1 = 0 ;
            int64_t val2 = 0 ;
            BinaryOperator* new_instruction ;
            // The isa<> operator works exactly like the Java “instanceof” operator
            if (isa<ConstantInt>(operand1)) {
                val1 = dyn_cast<ConstantInt>(operand1)->getSExtValue();
            }
            if (isa<ConstantInt>(operand2)) {
                val2 = dyn_cast<ConstantInt>(operand2)->getSExtValue();
            }

            switch (Inst.getOpcode()) {
              case Instruction::Mul:
              // x*2^n = 2^n*x  
              // shift to left 
                if(isa<ConstantInt>(operand1)&&return_power(val1)!=-1){
                  new_instruction = BinaryOperator::Create( Instruction::Shl, 
                                                              operand2, 
                                                              ConstantInt::get(Inst.getType(), 
                                                              return_power(val1)),"shl",&Inst);
                  Inst.replaceAllUsesWith(new_instruction);
                  erase_list.push_back(&Inst);
                }else if(isa<ConstantInt>(operand2)&&return_power(val2)!=-1){
                  new_instruction = BinaryOperator::Create( Instruction::Shl, 
                                                              operand1, 
                                                              ConstantInt::get(Inst.getType(), 
                                                              return_power(val2)),"shl",&Inst);
                  Inst.replaceAllUsesWith(new_instruction);
                  erase_list.push_back(&Inst);
                }
              break; 
              case Instruction::SDiv:
              // x/1 = x 
              // shift to right 
                if(isa<ConstantInt>(operand2)&&return_power(val2)!=-1){
                  new_instruction = BinaryOperator::Create( Instruction::LShr, 
                                                              operand1, 
                                                              ConstantInt::get(Inst.getType(), 
                                                              return_power(val2), 0),"lshr",&Inst);
                  Inst.replaceAllUsesWith(operand1);
                  erase_list.push_back(&Inst);
                }
              break; 
          }
        }
      }
      for(auto &i:erase_list){
        i->eraseFromParent();
      }
    }
    return true;
  }
};  // class StrengthReduction

char StrengthReduction::ID = 0;
RegisterPass<StrengthReduction> X(
    "strength-reduction",
    "CSCD70: Strength Reduction");

}  // anonymous namespace
