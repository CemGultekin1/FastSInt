#include "midptransform.h"
#include <iostream>

MidpointTransform::MidpointTransform(Midpoint* midp,IncondenseMidpoint*imidp){
    _midp = midp;
    _imidp = imidp;
    _exit_ind = NLLC;
}

bool MidpointTransform::no_exit(){
    return _exit_ind == EOC;
}

IncondenseMidpoint* MidpointTransform::run_transform(){
    int_type* cm = _midp->_coords;
    int_type* cm_init = cm;
    int_type* c0 = _imidp->_coords;
    int_type new_coord = _midp->_node_id;

    float_type* wm = _midp->_weights;
    float_type* w0 = _imidp->_weights;

    float_type alpha = w0[_exit_ind_imidp]/wm[_exit_ind];


    if(alpha == 0){
        return _imidp;
    }
    while(*cm!=EOC && *c0!=EOC){
        while(*c0 == NLLC && *c0!=EOC){
            c0++;
            w0++;
        }
        if(*c0 == EOC){
            break;
        }
        if(*c0==*cm){
            *w0 = *w0 - alpha*(*wm);
            cm++;
            wm++;
        }
        c0++;
        w0++;
    }

    _imidp->_coords[_exit_ind_imidp] = NLLC;
    _imidp->concatenate(alpha,new_coord);

    return _imidp;
}

void MidpointTransform::all_exit_indices(float_type tol, int_type* mem){
    int_type* mem1 = mem;
    int_type* cm = _midp->_coords;
    int_type* cm_init = cm;
    int_type* c0 = _imidp->_coords;
    int_type new_coord = _midp->_node_id;

    float_type* wm = _midp->_weights;
    float_type* w0 = _imidp->_weights;

    float_type tstar = (w0[_exit_ind_imidp]-wm[_exit_ind])/wm[_exit_ind];
    float_type tstar1;
    while(*cm!=EOC && *c0!=EOC){
        while(*c0 == NLLC && *c0!=EOC){
            c0++;
            w0++;
        }
        if(*c0 == EOC){
            break;
        }
        if(*c0==*cm){
            tstar1 = (*w0 - *wm)/(*wm);            
            if(tstar1 - tstar < tol){
                *mem1 = cm - cm_init;
                mem1++;
            }
            cm++;
            wm++;
        }
        c0++;
        w0++;
    }
    *mem1 = EOC;
}

int_type MidpointTransform::exit_index(){
    if(_exit_ind != NLLC){
        return _exit_ind;
    }
    /*
        (y0 - ym)*t + ym = y(t)
        find smallest t>0 such that y(t) touches 0
        (while all y(t) are nonnegative for case of resimplexifying)
        here we assert that ym and y0 have all entries nonnegative
        if ym=0 and y0>0, then __never touches zero___
        if ym=0 and y0=0, then __never touches zero___
        if ym>0 and y0=0, then t=1
        if ym>0 and y0>0, then
            if ym>y0, then t=ym/(ym-y0) > 1
            if ym=y0, then __never touches zero___
            if ym<y0, then __never touches zero___
        if we find a coordinate in the midpoint, that doesn't exist in 
        the point, then t = 1, end of the story
    */
    float_type tstar; 
    float_type tstar1;
    bool tstar_init =  false;
    int_type imstar;
    int_type i0star;

    int_type* cm = _midp->_coords;
    int_type* cm_init = cm;
    int_type* c0 = _imidp->_coords;
    int_type* c0_init = c0;

    float_type* wm = _midp->_weights;
    float_type* w0 = _imidp->_weights;

    while(*cm!=EOC && *c0!=EOC){
        while(*c0 == NLLC && *c0!=EOC){
            c0++;
            w0++;
        }
        if(*c0 == EOC){
            break;
        }
        if(*c0==*cm){
            if(*w0 > 0){
                if(*wm > *w0){
                    // std::cout.precision(3);
                    // std::cout << std:: scientific;
                    tstar1 = *wm/(*wm - *w0);
                    // std::cout << *c0 << " tstar1 = " << tstar1 << " = " <<  *wm << "/(" <<  *wm  << " - " << *w0 << ")" << std::endl;
                    if(tstar_init){
                        if(tstar1 < tstar){
                            tstar = tstar1;
                            imstar = cm - cm_init;
                            i0star = c0 - c0_init;;
                        }
                    }else{
                        tstar = tstar1;
                        imstar = cm - cm_init;
                        i0star = c0 - c0_init;;
                        tstar_init = true;
                    }
                }
            }else{
                _exit_ind = cm - cm_init;
                _exit_ind_imidp = c0 - c0_init;
                return cm - cm_init;
            }
            cm++;
            wm++;
        }
        c0++;
        w0++;
    }
    if(tstar_init){
        _exit_ind = imstar;
        _exit_ind_imidp = i0star;
        return imstar;
    }else{
        _exit_ind = EOC;
        _exit_ind_imidp = EOC;
        return EOC;
    }
    
}

