// Generated by Bisonc++ V4.13.01 on Sat, 21 May 2016 01:30:01 +0100

// $insert class.ih
#include <zapata/json/JSONTokenizerimpl.h>

// The FIRST element of SR arrays shown below uses `d_type', defining the
// state's type, and `d_lastIdx' containing the last element's index. If
// d_lastIdx contains the REQ_TOKEN bitflag (see below) then the state needs
// a token: if in this state d_token__ is _UNDETERMINED_, nextToken() will be
// called

// The LAST element of SR arrays uses `d_token' containing the last retrieved
// token to speed up the (linear) seach.  Except for the first element of SR
// arrays, the field `d_action' is used to determine what to do next. If
// positive, it represents the next state (used with SHIFT); if zero, it
// indicates `ACCEPT', if negative, -d_action represents the number of the
// rule to reduce to.

// `lookup()' tries to find d_token__ in the current SR array. If it fails, and
// there is no default reduction UNEXPECTED_TOKEN__ is thrown, which is then
// caught by the error-recovery function.

// The error-recovery function will pop elements off the stack until a state
// having bit flag ERR_ITEM is found. This state has a transition on _error_
// which is applied. In this _error_ state, while the current token is not a
// proper continuation, new tokens are obtained by nextToken(). If such a
// token is found, error recovery is successful and the token is
// handled according to the error state's SR table and parsing continues.
// During error recovery semantic actions are ignored.

// A state flagged with the DEF_RED flag will perform a default
// reduction if no other continuations are available for the current token.

// The ACCEPT STATE never shows a default reduction: when it is reached the
// parser returns ACCEPT(). During the grammar
// analysis phase a default reduction may have been defined, but it is
// removed during the state-definition phase.

// So:
//      s_x[] = 
//      {
//                  [_field_1_]         [_field_2_]
//
// First element:   {state-type,        idx of last element},
// Other elements:  {required token,    action to perform},
//                                      ( < 0: reduce, 
//                                          0: ACCEPT,
//                                        > 0: next state)
// Last element:    {set to d_token__,    action to perform}
//      }

// When the --thread-safe option is specified, all static data are defined as
// const. If --thread-safe is not provided, the state-tables are not defined
// as const, since the lookup() function below will modify them


namespace // anonymous
{
    char const author[] = "Frank B. Brokken (f.b.brokken@rug.nl)";

    enum 
    {
        STACK_EXPANSION = 5     // size to expand the state-stack with when
                                // full
    };

    enum ReservedTokens
    {
        PARSE_ACCEPT     = 0,   // `ACCEPT' TRANSITION
        _UNDETERMINED_   = -2,
        _EOF_            = -1,
        _error_          = 256
    };
    enum StateType       // modify statetype/data.cc when this enum changes
    {
        NORMAL,
        ERR_ITEM,
        REQ_TOKEN,
        ERR_REQ,    // ERR_ITEM | REQ_TOKEN
        DEF_RED,    // state having default reduction
        ERR_DEF,    // ERR_ITEM | DEF_RED
        REQ_DEF,    // REQ_TOKEN | DEF_RED
        ERR_REQ_DEF // ERR_ITEM | REQ_TOKEN | DEF_RED
    };    
    struct PI__     // Production Info
    {
        size_t d_nonTerm; // identification number of this production's
                            // non-terminal 
        size_t d_size;    // number of elements in this production 
    };

    struct SR__     // Shift Reduce info, see its description above
    {
        union
        {
            int _field_1_;      // initializer, allowing initializations 
                                // of the SR s_[] arrays
            int d_type;
            int d_token;
        };
        union
        {
            int _field_2_;

            int d_lastIdx;          // if negative, the state uses SHIFT
            int d_action;           // may be negative (reduce), 
                                    // postive (shift), or 0 (accept)
            size_t d_errorState;    // used with Error states
        };
    };

    // $insert staticdata
    
// Productions Info Records:
PI__ const s_productionInfo[] = 
{
     {0, 0}, // not used: reduction values are negative
     {268, 1}, // 1: exp ->  object
     {268, 1}, // 2: exp ->  array
     {269, 4}, // 3: object (LCB) ->  LCB #0001 opt_pairlist RCB
     {272, 0}, // 4: #0001 ->  <empty>
     {270, 4}, // 5: array (LB) ->  LB #0002 opt_valuelist RB
     {274, 0}, // 6: #0002 ->  <empty>
     {271, 0}, // 7: opt_pairlist ->  <empty>
     {271, 1}, // 8: opt_pairlist ->  pairlist
     {275, 4}, // 9: pairlist (STRING) ->  STRING #0003 COLON value
     {276, 0}, // 10: #0003 ->  <empty>
     {275, 6}, // 11: pairlist (COMMA) ->  pairlist COMMA STRING #0004 COLON value
     {278, 0}, // 12: #0004 ->  <empty>
     {273, 0}, // 13: opt_valuelist ->  <empty>
     {273, 1}, // 14: opt_valuelist ->  valuelist
     {279, 1}, // 15: valuelist ->  value
     {279, 3}, // 16: valuelist (COMMA) ->  valuelist COMMA value
     {277, 1}, // 17: value ->  object
     {277, 1}, // 18: value ->  array
     {277, 1}, // 19: value (STRING) ->  STRING
     {277, 1}, // 20: value (BOOLEAN) ->  BOOLEAN
     {277, 1}, // 21: value (INTEGER) ->  INTEGER
     {277, 1}, // 22: value (DOUBLE) ->  DOUBLE
     {277, 1}, // 23: value (NIL) ->  NIL
     {280, 1}, // 24: exp_$ ->  exp
};

// State info and SR__ transitions for each state.


SR__ s_0[] =
{
    { { REQ_TOKEN}, { 6} },          
    { {       268}, { 1} }, // exp   
    { {       269}, { 2} }, // object
    { {       270}, { 3} }, // array 
    { {       262}, { 4} }, // LCB   
    { {       264}, { 5} }, // LB    
    { {         0}, { 0} },          
};

SR__ s_1[] =
{
    { { REQ_TOKEN}, {            2} }, 
    { {     _EOF_}, { PARSE_ACCEPT} }, 
    { {         0}, {            0} }, 
};

SR__ s_2[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -1} }, 
};

SR__ s_3[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -2} }, 
};

SR__ s_4[] =
{
    { { DEF_RED}, {  2} },         
    { {     272}, {  6} }, // #0001
    { {       0}, { -4} },         
};

SR__ s_5[] =
{
    { { DEF_RED}, {  2} },         
    { {     274}, {  7} }, // #0002
    { {       0}, { -6} },         
};

SR__ s_6[] =
{
    { { REQ_DEF}, {  4} },                
    { {     271}, {  8} }, // opt_pairlist
    { {     275}, {  9} }, // pairlist    
    { {     257}, { 10} }, // STRING      
    { {       0}, { -7} },                
};

SR__ s_7[] =
{
    { { REQ_DEF}, {  13} },                 
    { {     273}, {  11} }, // opt_valuelist
    { {     279}, {  12} }, // valuelist    
    { {     277}, {  13} }, // value        
    { {     269}, {  14} }, // object       
    { {     270}, {  15} }, // array        
    { {     257}, {  16} }, // STRING       
    { {     258}, {  17} }, // BOOLEAN      
    { {     259}, {  18} }, // INTEGER      
    { {     260}, {  19} }, // DOUBLE       
    { {     261}, {  20} }, // NIL          
    { {     262}, {   4} }, // LCB          
    { {     264}, {   5} }, // LB           
    { {       0}, { -13} },                 
};

SR__ s_8[] =
{
    { { REQ_TOKEN}, {  2} },       
    { {       263}, { 21} }, // RCB
    { {         0}, {  0} },       
};

SR__ s_9[] =
{
    { { REQ_DEF}, {  2} },         
    { {     266}, { 22} }, // COMMA
    { {       0}, { -8} },         
};

SR__ s_10[] =
{
    { { DEF_RED}, {   2} },         
    { {     276}, {  23} }, // #0003
    { {       0}, { -10} },         
};

SR__ s_11[] =
{
    { { REQ_TOKEN}, {  2} },      
    { {       265}, { 24} }, // RB
    { {         0}, {  0} },      
};

SR__ s_12[] =
{
    { { REQ_DEF}, {   2} },         
    { {     266}, {  25} }, // COMMA
    { {       0}, { -14} },         
};

SR__ s_13[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -15} }, 
};

SR__ s_14[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -17} }, 
};

SR__ s_15[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -18} }, 
};

SR__ s_16[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -19} }, 
};

SR__ s_17[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -20} }, 
};

SR__ s_18[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -21} }, 
};

SR__ s_19[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -22} }, 
};

SR__ s_20[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -23} }, 
};

SR__ s_21[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -3} }, 
};

SR__ s_22[] =
{
    { { REQ_TOKEN}, {  2} },          
    { {       257}, { 26} }, // STRING
    { {         0}, {  0} },          
};

SR__ s_23[] =
{
    { { REQ_TOKEN}, {  2} },         
    { {       267}, { 27} }, // COLON
    { {         0}, {  0} },         
};

SR__ s_24[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -5} }, 
};

SR__ s_25[] =
{
    { { REQ_TOKEN}, { 11} },           
    { {       277}, { 28} }, // value  
    { {       269}, { 14} }, // object 
    { {       270}, { 15} }, // array  
    { {       257}, { 16} }, // STRING 
    { {       258}, { 17} }, // BOOLEAN
    { {       259}, { 18} }, // INTEGER
    { {       260}, { 19} }, // DOUBLE 
    { {       261}, { 20} }, // NIL    
    { {       262}, {  4} }, // LCB    
    { {       264}, {  5} }, // LB     
    { {         0}, {  0} },           
};

SR__ s_26[] =
{
    { { DEF_RED}, {   2} },         
    { {     278}, {  29} }, // #0004
    { {       0}, { -12} },         
};

SR__ s_27[] =
{
    { { REQ_TOKEN}, { 11} },           
    { {       277}, { 30} }, // value  
    { {       269}, { 14} }, // object 
    { {       270}, { 15} }, // array  
    { {       257}, { 16} }, // STRING 
    { {       258}, { 17} }, // BOOLEAN
    { {       259}, { 18} }, // INTEGER
    { {       260}, { 19} }, // DOUBLE 
    { {       261}, { 20} }, // NIL    
    { {       262}, {  4} }, // LCB    
    { {       264}, {  5} }, // LB     
    { {         0}, {  0} },           
};

SR__ s_28[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -16} }, 
};

SR__ s_29[] =
{
    { { REQ_TOKEN}, {  2} },         
    { {       267}, { 31} }, // COLON
    { {         0}, {  0} },         
};

SR__ s_30[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -9} }, 
};

SR__ s_31[] =
{
    { { REQ_TOKEN}, { 11} },           
    { {       277}, { 32} }, // value  
    { {       269}, { 14} }, // object 
    { {       270}, { 15} }, // array  
    { {       257}, { 16} }, // STRING 
    { {       258}, { 17} }, // BOOLEAN
    { {       259}, { 18} }, // INTEGER
    { {       260}, { 19} }, // DOUBLE 
    { {       261}, { 20} }, // NIL    
    { {       262}, {  4} }, // LCB    
    { {       264}, {  5} }, // LB     
    { {         0}, {  0} },           
};

SR__ s_32[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -11} }, 
};


// State array:
SR__ *s_state[] =
{
  s_0,  s_1,  s_2,  s_3,  s_4,  s_5,  s_6,  s_7,  s_8,  s_9,
  s_10,  s_11,  s_12,  s_13,  s_14,  s_15,  s_16,  s_17,  s_18,  s_19,
  s_20,  s_21,  s_22,  s_23,  s_24,  s_25,  s_26,  s_27,  s_28,  s_29,
  s_30,  s_31,  s_32,
};

} // anonymous namespace ends


// $insert namespace-open
namespace zpt
{

// If the parsing function call uses arguments, then provide an overloaded
// function.  The code below doesn't rely on parameters, so no arguments are
// required.  Furthermore, parse uses a function try block to allow us to do
// ACCEPT and ABORT from anywhere, even from within members called by actions,
// simply throwing the appropriate exceptions.

JSONTokenizerBase::JSONTokenizerBase()
:
    d_stackIdx__(-1),
    // $insert debuginit
    d_debug__(false),
    d_nErrors__(0),
    // $insert requiredtokens
    d_requiredTokens__(0),
    d_acceptedTokens__(d_requiredTokens__),
    d_token__(_UNDETERMINED_),
    d_nextToken__(_UNDETERMINED_)
{}


void JSONTokenizer::print__()
{
// $insert print
}

void JSONTokenizerBase::clearin()
{
    d_token__ = d_nextToken__ = _UNDETERMINED_;
}

void JSONTokenizerBase::push__(size_t state)
{
    if (static_cast<size_t>(d_stackIdx__ + 1) == d_stateStack__.size())
    {
        size_t newSize = d_stackIdx__ + STACK_EXPANSION;
        d_stateStack__.resize(newSize);
        d_valueStack__.resize(newSize);
    }
    ++d_stackIdx__;
    d_stateStack__[d_stackIdx__] = d_state__ = state;
    *(d_vsp__ = &d_valueStack__[d_stackIdx__]) = d_val__;
}

void JSONTokenizerBase::popToken__()
{
    d_token__ = d_nextToken__;

    d_val__ = d_nextVal__;
    d_nextVal__ = STYPE__();

    d_nextToken__ = _UNDETERMINED_;
}
     
void JSONTokenizerBase::pushToken__(int token)
{
    d_nextToken__ = d_token__;
    d_nextVal__ = d_val__;
    d_token__ = token;
}
     
void JSONTokenizerBase::pop__(size_t count)
{
    if (d_stackIdx__ < static_cast<int>(count))
    {
        ABORT();
    }

    d_stackIdx__ -= count;
    d_state__ = d_stateStack__[d_stackIdx__];
    d_vsp__ = &d_valueStack__[d_stackIdx__];
}

inline size_t JSONTokenizerBase::top__() const
{
    return d_stateStack__[d_stackIdx__];
}

void JSONTokenizer::executeAction(int production)
try
{
    if (d_token__ != _UNDETERMINED_)
        pushToken__(d_token__);     // save an already available token

// $insert defaultactionreturn
                            // save default non-nested block $$
    if (int size = s_productionInfo[production].d_size)
        d_val__ = d_vsp__[1 - size];

    switch (production)
    {
        // $insert actioncases
        
        case 1:
        {
         d_scanner.result(zpt::JSObject);
         }
        break;

        case 2:
        {
         d_scanner.result(zpt::JSArray);
         }
        break;

        case 3:
        {
         d_scanner.finish(zpt::JSObject);
         }
        break;

        case 4:
        d_val__ = d_vsp__[0];
        {
         d_scanner.init(zpt::JSObject);
         }
        break;

        case 5:
        {
         d_scanner.finish(zpt::JSArray);
         }
        break;

        case 6:
        d_val__ = d_vsp__[0];
        {
         d_scanner.init(zpt::JSArray);
         }
        break;

        case 8:
        {
         }
        break;

        case 9:
        {
         d_scanner.add();
         }
        break;

        case 10:
        d_val__ = d_vsp__[0];
        {
         d_scanner.init(zpt::JSObject, d_scanner.matched());
         }
        break;

        case 11:
        {
         d_scanner.add();
         }
        break;

        case 12:
        d_val__ = d_vsp__[-2];
        {
         d_scanner.init(zpt::JSObject, d_scanner.matched());
         }
        break;

        case 14:
        {
         }
        break;

        case 15:
        {
         d_scanner.add();
         }
        break;

        case 16:
        {
         d_scanner.add();
         }
        break;

        case 17:
        {
         }
        break;

        case 18:
        {
         }
        break;

        case 19:
        {
         string _out(d_scanner.matched());
         d_scanner.init(_out);
         }
        break;

        case 20:
        {
         bool _out;
         string _in(d_scanner.matched());
         zpt::fromstr(_in, &_out);
         d_scanner.init(_out);
         }
        break;

        case 21:
        {
         long long _out;
         string _in(d_scanner.matched());
         zpt::fromstr(_in, &_out);
         d_scanner.init(_out);
         }
        break;

        case 22:
        {
         double _out;
         string _in(d_scanner.matched());
         zpt::fromstr(_in, &_out);
         d_scanner.init(_out);
         }
        break;

        case 23:
        {
         d_scanner.init();
         }
        break;

    }
}
catch (std::exception const &exc)
{
    exceptionHandler__(exc);
}

inline void JSONTokenizerBase::reduce__(PI__ const &pi)
{
    d_token__ = pi.d_nonTerm;
    pop__(pi.d_size);

}

// If d_token__ is _UNDETERMINED_ then if d_nextToken__ is _UNDETERMINED_ another
// token is obtained from lex(). Then d_nextToken__ is assigned to d_token__.
void JSONTokenizer::nextToken()
{
    if (d_token__ != _UNDETERMINED_)        // no need for a token: got one
        return;                             // already

    if (d_nextToken__ != _UNDETERMINED_)
    {
        popToken__();                       // consume pending token
    }
    else
    {
        ++d_acceptedTokens__;               // accept another token (see
                                            // errorRecover())
        d_token__ = lex();
        if (d_token__ <= 0)
            d_token__ = _EOF_;
    }
    print();
}

// if the final transition is negative, then we should reduce by the rule
// given by its positive value. Note that the `recovery' parameter is only
// used with the --debug option
int JSONTokenizer::lookup(bool recovery)
{
// $insert threading
    SR__ *sr = s_state[d_state__];          // get the appropriate state-table
    int lastIdx = sr->d_lastIdx;            // sentinel-index in the SR__ array
    
    SR__ *lastElementPtr = sr + lastIdx;
    lastElementPtr->d_token = d_token__;    // set search-token
    
    SR__ *elementPtr = sr + 1;              // start the search at s_xx[1]
    while (elementPtr->d_token != d_token__)
        ++elementPtr;
    

    if (elementPtr == lastElementPtr)   // reached the last element
    {
        if (elementPtr->d_action < 0)   // default reduction
        {
            return elementPtr->d_action;                
        }

        // No default reduction, so token not found, so error.
        throw UNEXPECTED_TOKEN__;
    }

    // not at the last element: inspect the nature of the action
    // (< 0: reduce, 0: ACCEPT, > 0: shift)

    int action = elementPtr->d_action;


    return action;
}

    // When an error has occurred, pop elements off the stack until the top
    // state has an error-item. If none is found, the default recovery
    // mode (which is to abort) is activated. 
    //
    // If EOF is encountered without being appropriate for the current state,
    // then the error recovery will fall back to the default recovery mode.
    // (i.e., parsing terminates)
void JSONTokenizer::errorRecovery()
try
{
    if (d_acceptedTokens__ >= d_requiredTokens__)// only generate an error-
    {                                           // message if enough tokens 
        ++d_nErrors__;                          // were accepted. Otherwise
        error("Syntax error");                  // simply skip input

    }


    // get the error state
    while (not (s_state[top__()][0].d_type & ERR_ITEM))
    {
        pop__();
    }

    // In the error state, lookup a token allowing us to proceed.
    // Continuation may be possible following multiple reductions,
    // but eventuall a shift will be used, requiring the retrieval of
    // a terminal token. If a retrieved token doesn't match, the catch below 
    // will ensure the next token is requested in the while(true) block
    // implemented below:

    int lastToken = d_token__;                  // give the unexpected token a
                                                // chance to be processed
                                                // again.

    pushToken__(_error_);                       // specify _error_ as next token
    push__(lookup(true));                       // push the error state

    d_token__ = lastToken;                      // reactivate the unexpected
                                                // token (we're now in an
                                                // ERROR state).

    bool gotToken = true;                       // the next token is a terminal

    while (true)
    {
        try
        {
            if (s_state[d_state__]->d_type & REQ_TOKEN)
            {
                gotToken = d_token__ == _UNDETERMINED_;
                nextToken();                    // obtain next token
            }
            
            int action = lookup(true);

            if (action > 0)                 // push a new state
            {
                push__(action);
                popToken__();

                if (gotToken)
                {

                    d_acceptedTokens__ = 0;
                    return;
                }
            }
            else if (action < 0)
            {
                // no actions executed on recovery but save an already 
                // available token:
                if (d_token__ != _UNDETERMINED_)
                    pushToken__(d_token__);
 
                                            // next token is the rule's LHS
                reduce__(s_productionInfo[-action]); 
            }
            else
                ABORT();                    // abort when accepting during
                                            // error recovery
        }
        catch (...)
        {
            if (d_token__ == _EOF_)
                ABORT();                    // saw inappropriate _EOF_
                      
            popToken__();                   // failing token now skipped
        }
    }
}
catch (ErrorRecovery__)       // This is: DEFAULT_RECOVERY_MODE
{
    ABORT();
}

    // The parsing algorithm:
    // Initially, state 0 is pushed on the stack, and d_token__ as well as
    // d_nextToken__ are initialized to _UNDETERMINED_. 
    //
    // Then, in an eternal loop:
    //
    //  1. If a state does not have REQ_TOKEN no token is assigned to
    //     d_token__. If the state has REQ_TOKEN, nextToken() is called to
    //      determine d_nextToken__ and d_token__ is set to
    //     d_nextToken__. nextToken() will not call lex() unless d_nextToken__ is 
    //     _UNDETERMINED_. 
    //
    //  2. lookup() is called: 
    //     d_token__ is stored in the final element's d_token field of the
    //     state's SR_ array. 
    //
    //  3. The current token is looked up in the state's SR_ array
    //
    //  4. Depending on the result of the lookup() function the next state is
    //     shifted on the parser's stack, a reduction by some rule is applied,
    //     or the parsing function returns ACCEPT(). When a reduction is
    //     called for, any action that may have been defined for that
    //     reduction is executed.
    //
    //  5. An error occurs if d_token__ is not found, and the state has no
    //     default reduction. Error handling was described at the top of this
    //     file.

int JSONTokenizer::parse()
try 
{
    push__(0);                              // initial state
    clearin();                              // clear the tokens.

    while (true)
    {
        try
        {
            if (s_state[d_state__]->d_type & REQ_TOKEN)
                nextToken();                // obtain next token


            int action = lookup(false);     // lookup d_token__ in d_state__

            if (action > 0)                 // SHIFT: push a new state
            {
                push__(action);
                popToken__();               // token processed
            }
            else if (action < 0)            // REDUCE: execute and pop.
            {
                executeAction(-action);
                                            // next token is the rule's LHS
                reduce__(s_productionInfo[-action]); 
            }
            else 
                ACCEPT();
        }
        catch (ErrorRecovery__)
        {
            errorRecovery();
        }
    }
}
catch (Return__ retValue)
{
    return retValue;
}

// $insert namespace-close
}


