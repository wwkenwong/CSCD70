#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>


using namespace llvm;


namespace {

class AlgebraicIdentity final : public FunctionPass {
 public:
  static char ID;

  AlgebraicIdentity() : FunctionPass(ID) {}
  virtual ~AlgebraicIdentity() override {}

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
      // Inst
      // Cannot erase during iteration 
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
            int64_t val1 =0 ;
            int64_t val2 =0 ;
            // The isa<> operator works exactly like the Java “instanceof” operator
            if (isa<ConstantInt>(operand1)) {
              //The dyn_cast<> operator is a “checking cast” operation. 
              //It checks to see if the operand is of the specified type,
              //and if so, returns a pointer to it (this operator does 
              //not work with references). 
              //If the operand is not of the correct type, a null pointer is returned
              // dyn_cast<ConstantInt>(operand1) returns a pointer 
                val1 = dyn_cast<ConstantInt>(operand1)->getSExtValue();
            }
            if (isa<ConstantInt>(operand2)) {
                val2 = dyn_cast<ConstantInt>(operand2)->getSExtValue();
            }

            switch (Inst.getOpcode()) {
              case Instruction::Add:
                // x+0 
                if(val1==0&&isa<ConstantInt>(operand1)){
                  // https://github.com/seahorn/llvm-dsa/blob/master/lib/AssistDS/TypeChecks.cpp#L735
                  //https://github.com/goyalankit/compilers-and-llvm/blob/master/spring2014-compilers/assignment2/LocalOpts/LocalOpts.cpp#L130
                  Inst.replaceAllUsesWith(operand2);
                  erase_list.push_back(&Inst);
                }else if (val2==0&&isa<ConstantInt>(operand2))
                {
                  // 0+x
                  Inst.replaceAllUsesWith(operand1);
                  erase_list.push_back(&Inst);
                }
                break; 

              case Instruction::Sub:
                // x-0
                if(val2==0&&isa<ConstantInt>(operand2)){
                  Inst.replaceAllUsesWith(operand1);
                  outs() << "Triggered x-0 cases " << "\n";
                  erase_list.push_back(&Inst);
                }else if (operand1==operand2)
                {
                  Inst.replaceAllUsesWith(ConstantInt::get(Inst.getType(), 0));
                  erase_list.push_back(&Inst);
                }
              break; 

              case Instruction::Mul:
              // x*1 = 1*x = x 
                if(val1==1&&isa<ConstantInt>(operand1)){
                  Inst.replaceAllUsesWith(operand2);
                  erase_list.push_back(&Inst);
                }else if(val2==1&&isa<ConstantInt>(operand2)){
                  Inst.replaceAllUsesWith(operand1);
                  erase_list.push_back(&Inst);
                }
              break; 

              case Instruction::SDiv:
              // x/1 = x 
                if(val2==1&&isa<ConstantInt>(operand2)){
                  Inst.replaceAllUsesWith(operand1);
                  erase_list.push_back(&Inst);
                }
              // x/x = 1 
                else if (operand1==operand2)
                {
                  Inst.replaceAllUsesWith(ConstantInt::get(Inst.getType(), 1));
                  erase_list.push_back(&Inst);
                  
                }
              break; 
            if(edited){
              outs() << "Replacing " << "\n";
              Inst.eraseFromParent();

            }
          }
        }
      }
      for(auto &i:erase_list){
        i->eraseFromParent();
      }
      
    }
    return true;
  }
};  // class AlgebraicIdentity

char AlgebraicIdentity::ID = 0;
RegisterPass<AlgebraicIdentity> X(
    "algebraic-identity",
    "CSCD70: Algebraic Identity");

}  // anonymous namespace
