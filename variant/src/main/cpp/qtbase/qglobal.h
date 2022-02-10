
#ifdef Q_COMPILER_DELETE_MEMBERS
#define Q_DECL_EQ_DELETE = delete
#else
#define Q_DECL_EQ_DELETE
#endif
/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define Q_DISABLE_COPY(Class)            \
  Class(const Class &) Q_DECL_EQ_DELETE; \
  Class &operator=(const Class &) Q_DECL_EQ_DELETE;