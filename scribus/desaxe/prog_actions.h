/*
 *  prog_actions.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */


#ifndef PROG_ACTIONS_H
#define PROG_ACTIONS_H

/*****

    Defines the following actions:

    Retrieve<D>( name )                             - -> D
    Store<D>( name, gen<D> )                        - -> -
    Remove( name )                                  - -> -
    If( gen<bool> )                                 - -> -
    IfNot( gen<bool> )                              - -> -
    IfEqual<D>( gen<D>, gen<D> )                    - -> -
    IfNotEqual<D>( gen<D>, gen<D> )                 - -> -
    IfLess<D>( gen<D>, gen<D> )                     - -> -
    IfLessEq<D>( gen<D>, gen<D> )                   - -> -
    IfGreater<D>( gen<D>, gen<D> )                  - -> -
    IfGreaterEq<D>( gen<D>, gen<D> )                - -> -
    Else()                                          - -> -
    EndIf()                                         - -> -
    Count( name n )                                 - -> -
    Counter( name )                                 - -> int

*****/


#endif