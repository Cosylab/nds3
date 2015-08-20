%module(directors="1") nds3

%feature("director") nds::Delegate

%{
#define SWIG_FILE_WITH_INIT
#include "../include/nds3/nds3.h"
%}

#define NDS3_API


%include <std_string.i>
%include "../include/nds3/ndsbase.h"
%include "../include/nds3/ndsnode.h"
%include "../include/nds3/ndsport.h"
%include "../include/nds3/ndspvbase.h"
%include "../include/nds3/ndsdelegate.h"
%include "../include/nds3/ndspvholddelegate.h"
%include "../include/nds3/ndsfactory.h"




