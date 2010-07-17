/**
 * \file
 * \brief 
 * Implementation of Red-Black Tree as described in 
 * Intorduction to Algorithms. Cormen et al. Mc Grow Hill. 1990. pp 263-280
 * 
 * The intention is to implement interval trees mentioned in the same book, after the red-black.
 * Interval are heavily based on red-black trees (most operations are the same). So, we begin first 
 * with implementing red-black!
 *
 * Authors:
 *      ? <?@?.?>
 * 
 * Copyright 2009-2009 Evangelos Katsikaros
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 */

#include <vector>
#include <utility>


#include "d2.h"
#include "interval.h"

namespace Geom{

// used only in pick_next( )
enum enum_add_to_group { 
    ADD_TO_GROUP_A = 0, 
    ADD_TO_GROUP_B
};


enum enum_split_strategy { 
    QUADRATIC_SPIT = 0,
    LINEAR_COST,
    TOTAL_STRATEGIES // this one must be the last item
};



template <typename T>
class pedantic_vector:public std::vector<T> {
public:
    pedantic_vector(size_t s=0) : std::vector<T>(s) {}
    T& operator[](unsigned i) {
        //assert(i >= 0);
        assert(i < std::vector<T>::size());
        return std::vector<T>::operator[](i);
    }
    T const& operator[](unsigned i) const {
        //assert(i >= 0);
        assert(i < std::vector<T>::size());
        return std::vector<T>::operator[](i);
    }
/*
    erase( std::vector<T>::iterator it ) {
        //assert(i >= 0);
        assert( it < std::vector<T>::size());
        return std::vector<T>::erase(it);
    }
*/
};

class RTreeNode;

class RTreeRecord_Leaf{
public:
    Rect bounding_box;
    int data;

    RTreeRecord_Leaf(): bounding_box(), data(0)
    {}

    RTreeRecord_Leaf(Rect bb, int d): bounding_box(bb), data(d)
    {}
};

class RTreeRecord_NonLeaf{
public:
    Rect bounding_box;
    RTreeNode* data;    

    RTreeRecord_NonLeaf(): bounding_box(), data(0)
    {}

    RTreeRecord_NonLeaf(Rect bb, RTreeNode* d): bounding_box(bb), data(d)
    {}
};

/*
R-Tree has 2 kinds of nodes
* Leaves which store:
  - the actual data
  - the bounding box of the data

* Non-Leaves which store:
  - a child node (data)
  - the bounding box of the child node

This causes some code duplication in rtree.cpp. There are 2 cases:
- we care whether we touch a leaf/non-leaf node, since we write data in the node, so we want to 
  write the correct thing (int or RTreeNode*)
- we do NOT care  whether we touch a leaf/non-leaf node, because we only read/write the bounding 
  boxes which is the same in both cases.

TODO:
A better design would eliminate the duplication in the 2nd case, but we can't avoid the 1st probably.
*/
class RTreeNode{    
public:
    // first: bounding box
    // second: "data" (leaf-node) or node (NON leaf-node)
    //pedantic_vector< RTreeRecord_Leaf > children_leaves; // if this is empty, then node is leaf-node
    //pedantic_vector< RTreeRecord_NonLeaf > children_nodes;  // if this is empty, then node is NON-leaf node

    std::vector< RTreeRecord_Leaf > children_leaves; // if this is empty, then node is leaf-node
    std::vector< RTreeRecord_NonLeaf > children_nodes;  // if this is empty, then node is NON-leaf node

    RTreeNode(): children_leaves(0), children_nodes(0)
    {}

};


class RTree{
public:
    RTreeNode* root;

    // min/max records per node
    unsigned min_records;
    unsigned max_records; // allow +1 (used during insert)

    enum_split_strategy split_strategy;


    RTree( unsigned n, unsigned m, enum_split_strategy split_s ): 
        root(0), min_records( n ), max_records( m ), split_strategy( split_s ),
        tree_height(0)
    {}

    void insert( Rect const &r, unsigned shape);
    void search( const Rect &search_area, std::vector< int >* result, const RTreeNode* subtree ) const;
    //int erase( const RTreeRecord_Leaf & search );
    int erase( const Rect &search_area, const int shape_to_delete );

//  update

    void print_tree(RTreeNode* subtree_root, int depth ) const;

private:
    unsigned tree_height; // 0 is the root level

    void insert(  //Rect const &r, 
                //int shape, 
                const RTreeRecord_Leaf &leaf_record,
                const bool &insert_high = false, 
                const unsigned &stop_height = 0,
                const RTreeRecord_NonLeaf &nonleaf_record = RTreeRecord_NonLeaf()
                );
    // I1
    RTreeNode* choose_node( const Rect &r, const bool &insert_high = false, const unsigned &stop_height=0 ) const;
    double find_waste_area( const Rect &a, const Rect &b ) const;
    double find_enlargement( const Rect &a, const Rect &b ) const;

    // I2
    std::pair<RTreeNode*, RTreeNode*> split_node( RTreeNode *s );
        // QUADRATIC_SPIT
    std::pair<RTreeNode*, RTreeNode*> quadratic_split( RTreeNode* s );
    std::pair<unsigned, unsigned> pick_seeds( RTreeNode* s ) const;
    std::pair<unsigned, enum_add_to_group>  pick_next( RTreeNode* group_a, RTreeNode* group_b, RTreeNode* s, std::vector<bool> &assigned_v );
        // others...

    // I3
    bool adjust_tree(   RTreeNode* position, 
                        std::pair<RTreeNode*, RTreeNode*>  &node_division, 
                        bool split_performed
                        );
    std::pair< RTreeNode*, bool > find_parent( RTreeNode* subtree_root, Rect search_area, RTreeNode* wanted ) const;

    void recalculate_bounding_box( RTreeNode* parent, RTreeNode* child, unsigned &child_in_parent );

    void copy_group_a_to_existing_node( RTreeNode *position, RTreeNode* group_a );
    RTreeRecord_NonLeaf create_nonleaf_record_from_rtreenode( Rect &new_entry_bounding, RTreeNode *rtreenode );
    RTreeRecord_Leaf create_leaf_record_from_rtreenode( Rect &new_entry_bounding, RTreeNode *rtreenode );

    // erase
//    RTreeNode* find_leaf( RTreeNode* subtree, const RTreeRecord_Leaf &search ) const;
    RTreeNode* find_leaf( RTreeNode* subtree, const Rect &search_area, const int shape_to_delete ) const;

    bool condense_tree( RTreeNode* position
    //                  std::pair<RTreeNode*, RTreeNode*>  &node_division, // modified: it holds the last split group
    //                  bool initial_split_performed, 
    //                    const unsigned min_nodes
    //                    const unsigned max_nodes 
                        );
    int remove_record_from_parent( RTreeNode* parent, RTreeNode* child );
    void sanity_check(RTreeNode* subtree_root, int depth, bool used_during_insert = false ) const;

};



}; //close namespace

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :
