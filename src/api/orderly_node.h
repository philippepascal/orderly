/*
 * Copyright 2009, Lloyd Hilaiel.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 * 
 *  3. Neither the name of Lloyd Hilaiel nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */ 

#ifndef __ORDERLY_NODE_H__
#define __ORDERLY_NODE_H__

#include "orderly_common.h"

typedef enum {
    orderly_node_empty,
    orderly_node_null,
    orderly_node_string,
    orderly_node_boolean,
    orderly_node_any,
    orderly_node_integer,
    orderly_node_number
} orderly_node_type;

#define ORDERLY_RANGE_LHS_INT 0x1
#define ORDERLY_RANGE_LHS_DOUBLE 0x2
#define ORDERLY_RANGE_HAS_LHS(r) ((r).info & (ORDERLY_RANGE_LHS_INT | ORDERLY_RANGE_LHS_DOUBLE))

#define ORDERLY_RANGE_RHS_INT 0x4
#define ORDERLY_RANGE_RHS_DOUBLE 0x8
#define ORDERLY_RANGE_HAS_RHS(r) ((r).info & (ORDERLY_RANGE_RHS_INT | ORDERLY_RANGE_RHS_DOUBLE))

#define ORDERLY_RANGE_SPECIFIED(r) ((r).info != 0)


typedef struct 
{
    /** a bitfield specifying wether the RHS
     *  and LHS were provided, and what form they're in */
    unsigned int info;
    union {
        long int i;
        double d;
    } lhs;
    union {
        long int i;
        double d;
    } rhs;
} orderly_range;
    

typedef struct orderly_node_t {
    orderly_node_type t;
    const char * name;
    /* a json array of possible values
     * XXX: sure not doing our validators any favors by leaving
     *      this as a blob of text.  otoh, we'd orderly to be
     *      *dependent* on a means of parsing and representing json
     *      to make this more convenient */
    const char * values;
    /* a json representation of this members default value */
    const char * default_value;
    /* Does thes existence of this element require any others? */
    const char * requires;
    /* regular expression constraining allowable values
     * (optional for string nodes) */
    const char * regex;
    /* is this node optional? */
    unsigned int optional;
    /* range specifications for nodes that support it
     * (i.e. string {0,10} foo;) */
    orderly_range range;
    /* If this node has children (in the case of arrays, unions, or
     * objects), the first child is reference here and subsequent
     * children are referenced via the first child's sibling ptr */
    struct orderly_node_t * child;
    /* children of a common parent are linked via this pointer, flowing
     * from the first child to the last */
    struct orderly_node_t * sibling;
} orderly_node;

void orderly_free_node(orderly_alloc_funcs * alloc,
                       orderly_node ** node);

orderly_node * orderly_alloc_node(orderly_alloc_funcs * alloc,
                                  orderly_node_type t);

#endif