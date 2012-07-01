#define NEW_EXP(cls, e...)                                      \
   cls##Ptr(new cls(BlockScopePtr(), getLocation(), ##e))

