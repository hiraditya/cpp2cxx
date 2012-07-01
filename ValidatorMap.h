#ifndef VALIDATORMAP_H
#define VALIDATORMAP_H

#include "ExceptionHandler.h"
#include "set_utils.hpp"

#include <string>
#include <set>
#include <iostream>
#include <fstream>

typedef std::set<std::string> ValidMacros_t;

class ValidatorMap{

public:
    void InitValidatorMap(std::string const& v_file)
    {
        validator_file = v_file;
        std::ifstream p_file(validator_file);
        if(!p_file.is_open())
            throw ExceptionHandler("could not open the validator file");
        std::string dummy;
        int val;
        while(p_file.good()){
            std::string macro_id;
            //#define macro_switch 1
            p_file>>dummy; //#define
            p_file>>macro_id; //macro_id
            p_file>>val;      //replacement text
            validated_macros.insert(macro_id);
        }
        using namespace general_utilities;
#ifdef DEBUG_VALIDATOR
        std::cout<<"\nMacro switches are:\n";
        std::cout<<validated_macros;
#endif
    }

    ValidMacros_t const& GetValidMacros()const
    {
        return validated_macros;
    }

private:
    ValidMacros_t validated_macros;
    std::string validator_file;
};

#endif // VALIDATORMAP_H
