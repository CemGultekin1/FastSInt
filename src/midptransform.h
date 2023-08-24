#include "midpoint.h"

class MidpointTransform{
    Midpoint* _midp;
    IncondenseMidpoint* _imidp;
    public:
        int_type _exit_ind;        
        int_type _exit_ind_imidp;        
        MidpointTransform(Midpoint*,IncondenseMidpoint*);
        void all_exit_indices(float_type,int_type*);
        int_type exit_index();
        bool no_exit();
        IncondenseMidpoint* run_transform();
};