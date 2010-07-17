#include <rtree.h>
#include <limits>

/*
Based on source (BibTex):
@inproceedings{DBLP:conf/sigmod/Guttman84,
  author    = {Antonin Guttman},
  title     = {R-Trees: A Dynamic Index Structure for Spatial Searching},
  booktitle = {SIGMOD Conference},
  year      = {1984},
  pages     = {47-57},
  ee        = {http://doi.acm.org/10.1145/602259.602266, db/conf/sigmod/Guttman84.html},
}
*/

/*
#define _RTREE_PRINT(x) std::cout << x << std::endl;
#define _RTREE_PRINT_TREE( x, y ) print_tree( x, y );
#define _RTREE_PRINT_TREE_INS( x, y, z ) print_tree( x, y, z );
*/
//comment the following if you want output during RTree operations


#define _RTREE_PRINT(x) ; 
#define _RTREE_PRINT_TREE( x, y ) ;
#define _RTREE_PRINT_TREE_INS( x, y, z ) ;



/*
TODO 1
some    if(non_leaf)
        else // leaf
could be eliminated when function starts from a leaf
do leaf action
then repeat function for non-leafs only
candidates:
- adjust_tree
- condense_tree

TODO 2
generalize in a different way the splitting techniques

*/


namespace Geom{

/*=============================================================================
                            insert
===============================================================================
insert a new index entry E into the R-tree:

I1) find position of new record:
    choose_node will find a leaf node L (position) in which to place r
I2) add record to leaf node:
    if L has room for another entry install E
    else split_node will obtain L and LL containing E and all the old entries of L
    from the available splitting strategies we chose quadtratic-cost algorithm (just to begin 
    with sth)
    // TODO implement more of them
I3) propagate changes upward:
    Invoke adjust_tree on L, also passing LL if a split was performed.
I4) grow tree taller:
    if a node spilt propagation, cuased the root to split
        create new root whose children are the 2 resulting nodes
*/

void RTree::insert( Rect const &r, unsigned shape ){
    _RTREE_PRINT("\n=====================================");
    _RTREE_PRINT("insert");
    RTreeRecord_Leaf* leaf_record= new RTreeRecord_Leaf( r, shape );
    insert( *leaf_record );
}



void RTree::insert( const RTreeRecord_Leaf &leaf_record,
                    const bool &insert_high /* false */,
                    const unsigned &stop_height /* 0 */,
                    const RTreeRecord_NonLeaf &nonleaf_record /* 0 */
                )
{
    _RTREE_PRINT("\n--------------");
    _RTREE_PRINT("insert private. element:" << leaf_record.data << "  insert high:" << insert_high << "  stop height:" << stop_height );
    RTreeNode *position = 0;

    // if tree is unsued create the root Node, not described in source, stupid me :P
    if(root == 0){
        root = new RTreeNode();
    }

    _RTREE_PRINT("I1");     // I1
    if( insert_high == false ){ // choose leaf node 
        position = choose_node( leaf_record.bounding_box ); 
    }
    else { // choose nonleaf node
        position = choose_node( nonleaf_record.bounding_box, insert_high, stop_height ); 
    }
    _RTREE_PRINT("leaf node chosen: " );
    _RTREE_PRINT_TREE( position , 0 );
    std::pair< RTreeNode*, RTreeNode* > node_division;

    bool split_performed = false;

    if( position->children_nodes.size() > 0 ){ // non-leaf node: position
        // we must reach here only to insert high non leaf node, not insert leaf node 
        assert( insert_high == true ); 

        // put new element in node temporarily. Later on, if we need to, we will split the node.
        position->children_nodes.push_back( nonleaf_record ); 
        if( position->children_nodes.size() <= max_records ){
            _RTREE_PRINT("I2 nonleaf: no split: " << position->children_nodes.size() );     // I2
        }
        else{
            _RTREE_PRINT("I2 nonleaf: split: " << position->children_nodes.size() );     // I2
            node_division = split_node( position );
            split_performed = true;
        }

    }
    else { // leaf node: position: 
        // we must reach here only to insert leaf node, not insert high non leaf node
        assert( insert_high == false ); 


        // put new element in node temporarily. Later on, if we need to, we will split the node.
        position->children_leaves.push_back( leaf_record ); 
        if( position->children_leaves.size() <= max_records ){
            _RTREE_PRINT("I2 leaf: no split: " << position->children_leaves.size() );     // I2
        }
        else{
            _RTREE_PRINT("I2 leaf: split: " << position->children_leaves.size() << "  max_records:" << max_records);     // I2
            node_division = split_node( position );
            split_performed = true;
    
            _RTREE_PRINT("      group A");
            _RTREE_PRINT_TREE( node_division.first , 3 );
            _RTREE_PRINT("      group B");
            _RTREE_PRINT_TREE( node_division.second , 3 );
    
        }

    }

    _RTREE_PRINT("I3");    // I3    
    bool root_split_performed = adjust_tree( position, node_division, split_performed );
    _RTREE_PRINT("root split: " << root_split_performed);


//    _RTREE_PRINT("TREE:");
//    print_tree( root , 2 );

    _RTREE_PRINT("I4");    // I4
    if( root_split_performed ){
        std::pair<RTreeNode*, RTreeNode*> root_division;
        root_division = quadratic_split( root ); // AT5

        Rect first_record_bounding_box;
        Rect second_record_bounding_box;

        RTreeRecord_NonLeaf first_new_record = create_nonleaf_record_from_rtreenode( first_record_bounding_box, root_division.first );
        RTreeRecord_NonLeaf second_new_record = create_nonleaf_record_from_rtreenode( second_record_bounding_box, root_division.second ); 
        _RTREE_PRINT("          1st:");
        _RTREE_PRINT_TREE( first_new_record.data, 5 );
        _RTREE_PRINT("          2nd:");
        _RTREE_PRINT_TREE( second_new_record.data, 5 );

        // *new* root is by definition non-leaf. Install the new records there
        RTreeNode* new_root = new RTreeNode();
        new_root->children_nodes.push_back( first_new_record );
        new_root->children_nodes.push_back( second_new_record  );  

        delete root;

        root = new_root; 
        tree_height++; // increse tree height

        _RTREE_PRINT_TREE( root, 5 );
        sanity_check( root, 0 );
    }
    _RTREE_PRINT("done");

    /* 
        the node_division.second is saved on the tree
        the node_division.first was copied in existing tree in node
        so we don't need this anymore
    */ 
    delete node_division.first;
}

/* I1 =========================================================================

original: choose_node will find a leaf node L in which to place r
changed to choose_node will find a node L in which to place r
the node L is: 
non-leaf: if flag is set. the height of the node is insert_at_height
leaf: if flag is NOT set

1) Initialize: set N to be the root node
2) Leaf Check: 
    insert_height = false 
        if N is leaf return N
    insert_height = true
3) Choose subtree: If N not leaf OR not we are not in the proper height then   
    let F be an entry in N whose rect Fi needs least enlargement to incude r
    ties resolved with rect of smallest area
4) descend until a leaf is reached OR proper height is reached: set N to the child node pointed to by F and goto 2.
*/

// TODO keep stack with visited nodes

RTreeNode* RTree::choose_node( const Rect &r, const bool &insert_high /* false */, const unsigned &stop_height /* 0 */) const {

    _RTREE_PRINT("  CL1");// CL1
    RTreeNode *pos = root;

    double min_enlargement;
    double current_enlargement;
    int node_min_enlargement;
    unsigned current_height = 0; // zero is the root

    _RTREE_PRINT("  CL2   current_height:" << current_height << "  stop_height:" << stop_height << "  insert_high:" << insert_high);    
    // CL2 Leaf check && Height check
    while( ( insert_high ? true : pos->children_nodes.size() != 0  )   
        && ( insert_high ? current_height < stop_height : true ) ) 
            /* Leaf check, during insert leaf */
            /* node height check, during insert non-leaf */
    {
        _RTREE_PRINT("  CL3    current_height:" << current_height << "  stop_height:" << stop_height );    // CL3
        min_enlargement = std::numeric_limits<double>::max();
        current_enlargement = 0;
        node_min_enlargement = 0;

        for(unsigned i=0; i< pos->children_nodes.size(); i++){
            current_enlargement = find_enlargement( pos->children_nodes[i].bounding_box, r );

            // TODO tie not solved!
            if( current_enlargement < min_enlargement ){
                node_min_enlargement = i;
                min_enlargement = current_enlargement;
            }
        }
        _RTREE_PRINT("  CL4");    // CL4
        // descend to the node with the min_enlargement
        pos = pos->children_nodes[node_min_enlargement].data; 
        current_height++; // increase current visiting height
    }
    
    return pos;
}


/* 
find_enlargement:

enlargement that "a" needs in order to incude "b"
b is the new rect we want to insert.
a is the rect of the node we try to see if b should go in.
*/
double RTree::find_enlargement( const Rect &a, const Rect &b ) const{
    

    Rect union_rect(a);
    union_rect.unionWith(b);

    OptRect a_intersection_b = intersect( a, b );

    // a, b do not intersect
    if( a_intersection_b.isEmpty() ){ 
        _RTREE_PRINT("      find_enlargement (no intersect): " << union_rect.area() - a.area() - b.area() );
        return union_rect.area() - a.area() - b.area();
    }

    // a, b intersect

    // a contains b
    if( a.contains( b ) ){
        _RTREE_PRINT("      find_enlargement (intersect: a cont b): " << a.area() - b.area() );
        //return a.area() - b.area();
        return b.area() - a.area(); // enlargement is negative in this case. 
    }

    // b contains a
    if( b.contains( a ) ){
        _RTREE_PRINT("      find_enlargement (intersect: b cont a): " << a.area() - b.area() );
        return b.area() - a.area();
    }

    // a partialy cover b
    _RTREE_PRINT("      find_enlargement (intersect: a partial cover b): " << union_rect.area() - a.area() - b.area() - a_intersection_b->area() );
    return union_rect.area() 
            - ( a.area() - a_intersection_b->area() ) 
            - ( b.area() - a_intersection_b->area() );
}


/* I2 =========================================================================
use one split strategy
*/

std::pair<RTreeNode*, RTreeNode*> RTree::split_node( RTreeNode *s ){
/*
    if( split_strategy == LINEAR_COST ){
        linear_cost_split( ............. );
    }
*/
    return quadratic_split( s ); // else QUADRATIC_SPIT
}


/*-----------------------------------------------------------------------------
                                Quadratic Split

QS1) Pick first entry for each group:
    Appy pick_seeds to choose 2 entries to be the first elements of the groups. Assign each one of 
    them to one group
QS2) check if done:
    a) if all entries have been assinged stop
    b) if one group has so few entries that all the rest must be assignmed to it, in order for it to 
    have the min number , assign them and stop
QS3) select entry and assign:
    Inkvoke pick_next() to choose the next entry to assign. 
    *[in pick_next] Add it to the group whose covering rectangle will have to be enlrarged least to 
    accomodate it. Resolve ties by adding the entry to the group with the smaller are, then to the 
    one with fewer entries, then to either of the two.
    goto 2.
*/
std::pair<RTreeNode*, RTreeNode*> RTree::quadratic_split( RTreeNode *s ) {

    // s is the original leaf node or non-leaf node
    RTreeNode* group_a = new RTreeNode(); // a is the 1st group
    RTreeNode* group_b = new RTreeNode(); // b is the 2nd group


    _RTREE_PRINT("  QS1");     // QS1
    std::pair<unsigned, unsigned> initial_seeds;
    initial_seeds = pick_seeds(s);

    // if non-leaf node: s
    if( s->children_nodes.size() > 0 ){
        _RTREE_PRINT("  non-leaf node");         
        // each element is true if the node has been assinged to either "a" or "b"
        std::vector<bool> assigned_v( s->children_nodes.size() );
        std::fill( assigned_v.begin(), assigned_v.end(), false );

        group_a->children_nodes.push_back( s->children_nodes[initial_seeds.first] );
        assert(initial_seeds.first < assigned_v.size());
        assigned_v[ initial_seeds.first ] = true;

        group_b->children_nodes.push_back( s->children_nodes[initial_seeds.second] );
        assert(initial_seeds.second < assigned_v.size());
        assigned_v[ initial_seeds.second ] = true;

        _RTREE_PRINT("  QS2");     // QS2 
        unsigned num_of_not_assigned = s->children_nodes.size() - 2; 
        // so far we have assinged 2 out of all

        while( num_of_not_assigned ){// QS2 a
            _RTREE_PRINT("  QS2 b,   num_of_not_assigned:" << num_of_not_assigned);    // QS2 b
            /* 
                we are on NON leaf node so children of splitted groups must be nodes

                Check each group to see if one group has so few entries that all the rest must 
                be assignmed to it, in order for it to have the min number.
            */
            if( group_a->children_nodes.size() + num_of_not_assigned <= min_records ){
                // add the non-assigned to group_a
                for(unsigned i = 0; i < assigned_v.size(); i++){
                    if(assigned_v[i] == false){
                        group_a->children_nodes.push_back( s->children_nodes[i] );
                        assigned_v[i] = true;
                    }
                }
                break;           
            }

            if( group_b->children_nodes.size() + num_of_not_assigned <= min_records ){
                // add the non-assigned to group_b
                for( unsigned i = 0; i < assigned_v.size(); i++ ){
                    if( assigned_v[i] == false ){
                        group_b->children_nodes.push_back( s->children_nodes[i] );
                        assigned_v[i] = true;
                    }
                }
                break;
            }

            _RTREE_PRINT("  QS3");     // QS3
            std::pair<unsigned, enum_add_to_group>  next_element;
            next_element = pick_next( group_a, group_b, s, assigned_v );
            if( next_element.second == ADD_TO_GROUP_A ){
                group_a->children_nodes.push_back( s->children_nodes[ next_element.first ] );
            }   
            else{
                group_b->children_nodes.push_back( s->children_nodes[ next_element.first ] );
            }

            num_of_not_assigned--;
        }
    }
    // else leaf node: s
    else{
        _RTREE_PRINT("  leaf node"); 
        // each element is true if the node has been assinged to either "a" or "b"
        std::vector<bool> assigned_v( s->children_leaves.size() );
        std::fill( assigned_v.begin(), assigned_v.end(), false );

        // assign 1st seed to group a
        group_a->children_leaves.push_back( s->children_leaves[initial_seeds.first] );
        assert(initial_seeds.first < assigned_v.size());
        assigned_v[ initial_seeds.first ] = true;

        // assign 2nd seed to group b
        group_b->children_leaves.push_back( s->children_leaves[initial_seeds.second] );
        assert(initial_seeds.second < assigned_v.size());
        assigned_v[ initial_seeds.second ] = true;

        _RTREE_PRINT("  QS2");    // QS2 
        unsigned num_of_not_assigned = s->children_leaves.size() - 2; 
        // so far we have assinged 2 out of all

        while( num_of_not_assigned ){// QS2 a
            _RTREE_PRINT("  QS2 b,   num_of_not_assigned:" << num_of_not_assigned);    // QS2 b
            /* 
                we are on leaf node so children of splitted groups must be leaves

                Check each group to see if one group has so few entries that all the rest must 
                be assignmed to it, in order for it to have the min number.
            */
            if( group_a->children_leaves.size() + num_of_not_assigned <= min_records ){
                _RTREE_PRINT("  add the non-assigned to group_a");    
                // add the non-assigned to group_a
                for( unsigned i = 0; i < assigned_v.size(); i++ ){
                    if( assigned_v[i] == false ){
                        group_a->children_leaves.push_back( s->children_leaves[i] );
                        assigned_v[i] = true;
                    }
                }
                break;           
            }

            if( group_b->children_leaves.size() + num_of_not_assigned <= min_records ){
                _RTREE_PRINT("  add the non-assigned to group_b");    
                // add the non-assigned to group_b
                for( unsigned i = 0; i < assigned_v.size(); i++ ){
                    if( assigned_v[i] == false ){
                        group_b->children_leaves.push_back( s->children_leaves[i] );
                        assigned_v[i] = true;
                    }
                }
                break;
            }

            _RTREE_PRINT("  QS3");    // QS3
            std::pair<unsigned, enum_add_to_group>  next_element;
            next_element = pick_next(group_a, group_b, s, assigned_v);
            if( next_element.second == ADD_TO_GROUP_A ){
                group_a->children_leaves.push_back( s->children_leaves[ next_element.first ] );
            }   
            else{
                group_b->children_leaves.push_back( s->children_leaves[ next_element.first ] );
            }

            num_of_not_assigned--;
        }
    }
    assert( initial_seeds.first != initial_seeds.second );
    return std::make_pair( group_a, group_b );
}

/*
PS1) caclulate ineffeciency of grouping entries together:
    Foreach pair of entries E1 (i), E2 (j) compose rectangle J (i_union_j) inlcuding E1, E2. 
    Calculate d = area(i_union_j) - area(i) - area(j)
PS2) choose the most wastefull pair:
    Choose pair with largest d
*/

std::pair<unsigned, unsigned> RTree::pick_seeds( RTreeNode *s ) const{
    double current_d = 0;
    double max_d = std::numeric_limits<double>::min();
    unsigned seed_a = 0;
    unsigned seed_b = 1;
    _RTREE_PRINT("      pick_seeds");  

    // if non leaf node: s
    if( s->children_nodes.size() > 0 ){
        _RTREE_PRINT("      non leaf");    
        _RTREE_PRINT("      PS1");    // PS1
        for( unsigned a = 0; a < s->children_nodes.size(); a++ ){
            // with j=i we check only the upper (diagonal) half  
            // with j=i+1 we also avoid checking for b==a (we don't need it)
            for( unsigned b = a+1; b < s->children_nodes.size(); b++ ){
                _RTREE_PRINT("      PS2 " << a << " - " << b );    // PS2
                current_d = find_waste_area( s->children_nodes[a].bounding_box, s->children_nodes[b].bounding_box );

                if( current_d > max_d ){
                    max_d = current_d;
                    seed_a = a;
                    seed_b = b;
                }            
            }
        }
    }
    // else leaf node: s
    else{
        _RTREE_PRINT("      leaf node");
        _RTREE_PRINT("      PS1");    // PS1
        for( unsigned a = 0; a < s->children_leaves.size(); a++ ){
            // with j=i we check only the upper (diagonal) half  
            // with j=i+1 we also avoid checking for j==i (we don't need this one)
            for( unsigned b = a+1; b < s->children_leaves.size(); b++ ){
                _RTREE_PRINT("      PS2 " << s->children_leaves[a].data << ":" << s->children_leaves[a].bounding_box.area() 
                        <<  " - " << s->children_leaves[b].data << ":" << s->children_leaves[b].bounding_box.area() );    // PS2
                current_d = find_waste_area( s->children_leaves[a].bounding_box, s->children_leaves[b].bounding_box );

                if( current_d > max_d ){
                    max_d = current_d;
                    seed_a = a;
                    seed_b = b;
                }            
            }
        }
    }
    _RTREE_PRINT("      seed_a: " << seed_a << " seed_b:  " << seed_b );
    return std::make_pair( seed_a, seed_b );
}

/*
find_waste_area (used in pick_seeds step 1)

for a pair A, B compose a rect union_rect that includes a and b
calculate area of union_rect - area of a - area b
*/
double RTree::find_waste_area( const Rect &a, const Rect &b ) const{
    Rect union_rect(a);
    union_rect.unionWith(b);

    return union_rect.area() - a.area() - b.area();
}

/*
pick_next:
select one remaining entry for classification in a group

PN1) Determine cost of putting each entry in each group:
    Foreach entry E not yet in a group, calulate 
    d1= area increase required in the cover rect of Group 1 to inlcude E
    d2= area increase required in the cover rect of Group 2 to inlcude E
PN2) Find entry with greatest preference for each group:
    Choose any entry with the maximum difference between d1 and d2

*/

std::pair<unsigned, enum_add_to_group> RTree::pick_next(    RTreeNode* group_a, 
                                                            RTreeNode* group_b, 
                                                            RTreeNode* s, 
                                                            std::vector<bool> &assigned_v )
{
    double max_increase_difference = std::numeric_limits<double>::min();
    unsigned max_increase_difference_node = 0;
    double current_increase_difference = 0; 

    enum_add_to_group group_to_add;

    /*
    bounding boxes of the 2 new groups. This info isn't available, since they 
    have no parent nodes (so that the parent node knows the bounding box). 
    */
    Rect bounding_box_a;
    Rect bounding_box_b;

    double increase_area_a = 0;
    double increase_area_b = 0;

    _RTREE_PRINT("      pick_next,  assigned_v.size:" << assigned_v.size() );    

    // if non leaf node: one of the 2 groups (both groups are the same, either leaf/nonleaf)
    if( group_a->children_nodes.size() > 0 ){
        _RTREE_PRINT("      non leaf");    

        // calculate the bounding boxes of the 2 new groups.
        bounding_box_a = Rect( group_a->children_nodes[0].bounding_box );
        for( unsigned i = 1; i < group_a->children_nodes.size(); i++ ){
            bounding_box_a.unionWith( group_a->children_nodes[i].bounding_box );
        }

        bounding_box_b = Rect( group_b->children_nodes[0].bounding_box );
        for( unsigned i = 1; i < group_b->children_nodes.size(); i++ ){
            bounding_box_b.unionWith( group_b->children_nodes[i].bounding_box );
        }


        _RTREE_PRINT("      PN1");    // PN1
        for( unsigned i = 0; i < assigned_v.size(); i++ ){
            _RTREE_PRINT("      i:" << i << " assigned:" << assigned_v[i]);
            if( assigned_v[i] == false ){

                increase_area_a = find_enlargement( bounding_box_a, s->children_nodes[i].bounding_box );
                increase_area_b = find_enlargement( bounding_box_b, s->children_nodes[i].bounding_box );
                
                current_increase_difference = std::abs( increase_area_a - increase_area_b );
                _RTREE_PRINT("      PN2  " << i << ": " << current_increase_difference );    // PN2
                if( current_increase_difference > max_increase_difference ){
                    max_increase_difference = current_increase_difference;
                    max_increase_difference_node = i;

                    // TODO tie not solved!
                    if( increase_area_a < increase_area_b ){
                        group_to_add = ADD_TO_GROUP_A;
                    }
                    else{
                        group_to_add = ADD_TO_GROUP_B;
                    }
                }
            }
        }
        //assert(max_increase_difference_node >= 0);
        assert(max_increase_difference_node < assigned_v.size());
        assigned_v[max_increase_difference_node] = true;
        _RTREE_PRINT("      ... i:" << max_increase_difference_node << " assigned:" << assigned_v[max_increase_difference_node] );
    }
    else{     // else leaf node
        _RTREE_PRINT("      leaf");    

        // calculate the bounding boxes of the 2 new groups
        bounding_box_a = Rect( group_a->children_leaves[0].bounding_box );
        for( unsigned i = 1; i < group_a->children_leaves.size(); i++ ){
            std::cout<< " lala";
            bounding_box_a.unionWith( group_a->children_leaves[i].bounding_box );
        }

        bounding_box_b = Rect( group_b->children_leaves[0].bounding_box );
        for( unsigned i = 1; i < group_b->children_leaves.size(); i++ ){
            std::cout<< " lala";
            bounding_box_b.unionWith( group_b->children_leaves[i].bounding_box );
        }


        _RTREE_PRINT("      PN1");    // PN1
        for( unsigned i = 0; i < assigned_v.size(); i++ ){
            _RTREE_PRINT("      i:" << i << " assigned:" << assigned_v[i]);
            if( assigned_v[i] == false ){
                increase_area_a = find_enlargement( bounding_box_a, s->children_leaves[i].bounding_box );
                increase_area_b = find_enlargement( bounding_box_b, s->children_leaves[i].bounding_box );

                current_increase_difference = std::abs( increase_area_a - increase_area_b );
                _RTREE_PRINT("      PN2  " << i << ": " << current_increase_difference );    // PN2

                if( current_increase_difference > max_increase_difference ){
                    max_increase_difference = current_increase_difference;
                    max_increase_difference_node = i;

                    // TODO tie not solved!
                    if( increase_area_a < increase_area_b ){
                        group_to_add = ADD_TO_GROUP_A;
                    }
                    else{
                        group_to_add = ADD_TO_GROUP_B;
                    }
                }
            }
        }
        assert(max_increase_difference_node < assigned_v.size());
        assigned_v[max_increase_difference_node] = true;
        _RTREE_PRINT("      ... i:" << max_increase_difference_node << " assigned:" << assigned_v[max_increase_difference_node] );
    }

    _RTREE_PRINT("      node:" << max_increase_difference_node << " added:" << group_to_add );
    return std::make_pair( max_increase_difference_node, group_to_add );
}

/* I3 =========================================================================

adjust_tree:
Ascend from a leaf node L to root, adjusting covering rectangles and propagating node splits as
necessary

We modified this one from the source in the step AT1 and AT5

AT1) Initialize:
    Set N=L
    IF L was spilt previously, set NN to be the resulting second node AND
    (not mentioned in the original source but that's what it should mean)
    Assign all entries of first node to L
AT2) check if done:
    IF N is root stop
AT3) adjust covering rectangle in parent entry
    1) Let P be the parent of N
    2) Let EN be the N's entry in P
    3) Adjust EN bounding box so that it tightly enclosses all entry rectangles in N
AT4) Propagate node split upward
    IF N has a partner NN resulting from an earlier split 
        create a new entry ENN with ENN "p" pointing to NN and ENN bounding box enclosing all
        rectangles in NN

        IF there is room in P add ENN
        ELSE invoke split_node to produce P an PP containing ENN and all P's old entries.
AT5) Move up to next level
    Set N=P, 
    IF a split occurred, set NN=PP 
    goto AT1 (was goto AT2)
*/

bool RTree::adjust_tree(    RTreeNode* position, 
                            // modified: it holds the last split group
                            std::pair<RTreeNode*, RTreeNode*>  &node_division, 
                            bool initial_split_performed)
{
    RTreeNode* parent;
    unsigned child_in_parent; // the element in parent node that points to current posistion
    std::pair< RTreeNode*, bool > find_result;
    bool split_performed = initial_split_performed;
    bool root_split_performed = false;

    _RTREE_PRINT("  adjust_tree");   
    _RTREE_PRINT("  AT1");

    while( true ){
        _RTREE_PRINT("  ------- current tree status:");
        _RTREE_PRINT_TREE_INS(root, 2, true);

        // check for loop BREAK
        if( position == root ){
            _RTREE_PRINT("  AT2: found root");
            if( split_performed ){
                root_split_performed = true;
            }            
            break;
        } 

        if( split_performed ){
            copy_group_a_to_existing_node( position, node_division.first );
        }

        /* 
            pick randomly, let's say the 1st entry of the current node. 
            Search for this spatial area in the tree, and stop to the parent node.
            Then find position of current node pointer, in the parent node.
        */
        _RTREE_PRINT("  AT3.1");    // AT3.1    Let P be the parent of N
        if( position->children_nodes.size() > 0 ){
            find_result = find_parent( root, position->children_nodes[0].bounding_box, position);
        }
        else{
            find_result = find_parent( root, position->children_leaves[0].bounding_box, position);
        }
        parent = find_result.first;

        // parent is a non-leaf, by definition
        _RTREE_PRINT("  AT3.2");    // AT3.2    Let EN be the N's entry in P
        for( child_in_parent = 0; child_in_parent < parent->children_nodes.size(); child_in_parent++ ){
            if( parent->children_nodes[ child_in_parent ].data == position){
                _RTREE_PRINT("  child_in_parent: " << child_in_parent);
                break;
            }
        }
        
        _RTREE_PRINT("  AT3.3");    
        // AT3.2    Adjust EN bounding box so that it tightly enclosses all entry rectangles in N
        recalculate_bounding_box( parent, position, child_in_parent );


        _RTREE_PRINT("  AT4");    // AT4
        if( split_performed ){
            // create new record (from group_b) 
            //RTreeNode* new_node = new RTreeNode();
            Rect new_record_bounding;

            RTreeRecord_NonLeaf new_record = create_nonleaf_record_from_rtreenode( new_record_bounding, node_division.second );
            
            // install new entry (group_b)
            if( parent->children_nodes.size() < max_records ){
                parent->children_nodes.push_back( new_record );
                split_performed = false;
            }
            else{
                parent->children_nodes.push_back( new_record );
                node_division = quadratic_split( parent ); // AT5
                split_performed = true;
            }
           
        }
        _RTREE_PRINT("  AT5");    // AT5
        position = parent;
    }

    return root_split_performed;
}

/*
find_parent:
The source only mentions that we should "find" the parent. But it doesn't seay how. So we made a 
modification of search. 

Initially we take the root, a rect of the node, of which the parent we look for and the node we seek

We do a spatial search for this rect. If we find get an intersecttion with the rect we check if the
child is the one we look for.
If not we call find_parent again recursively
*/

std::pair< RTreeNode*, bool > RTree::find_parent( RTreeNode* subtree_root, 
                                                    Rect search_area, 
                                                    RTreeNode* wanted) const
{
    _RTREE_PRINT("find_parent");

    std::pair< RTreeNode*, bool > result;   
    if( subtree_root->children_nodes.size() > 0 ){
        
        for( unsigned i=0; i < subtree_root->children_nodes.size(); i++ ){
            if( subtree_root->children_nodes[i].data == wanted){
                _RTREE_PRINT("FOUND!!");     // non leaf node
                return std::make_pair( subtree_root, true );
            }

            if( subtree_root->children_nodes[i].bounding_box.intersects( search_area ) ){
                result = find_parent( subtree_root->children_nodes[i].data, search_area, wanted);
                if ( result.second ){
                    break;
                }
            }
        }
    }
    return result;
}


void RTree::copy_group_a_to_existing_node( RTreeNode *position, RTreeNode* group_a ){
    // clear position (the one that was split) and put there all the nodes of group_a
    if( position->children_nodes.size() > 0 ){
        _RTREE_PRINT("  copy_group...(): install group A to existing non-leaf node");    
        // non leaf-node: position
        position->children_nodes.clear();
        for( unsigned i=0; i < group_a->children_nodes.size(); i++ ){
            position->children_nodes.push_back( group_a->children_nodes[i] );
        }
    }
    else{
        _RTREE_PRINT("  copy_group...(): install group A to existing leaf node");    
        // leaf-node: positions
        position->children_leaves.clear();
        for( unsigned i=0; i < group_a->children_leaves.size(); i++ ){
            position->children_leaves.push_back( group_a->children_leaves[i] );
        }
    }
}



RTreeRecord_NonLeaf RTree::create_nonleaf_record_from_rtreenode( Rect &new_entry_bounding, RTreeNode* rtreenode ){

    if( rtreenode->children_nodes.size() > 0 ){
        // found bounding box of new entry
        new_entry_bounding = Rect( rtreenode->children_nodes[0].bounding_box );
        for(unsigned i = 1; i < rtreenode->children_nodes.size(); i++ ){
            new_entry_bounding.unionWith( rtreenode->children_nodes[ i ].bounding_box );
        }
    }
    else{  // non leaf: rtreenode
        // found bounding box of new entry
        new_entry_bounding = Rect( rtreenode->children_leaves[0].bounding_box );
        for(unsigned i = 1; i < rtreenode->children_leaves.size(); i++ ){
            new_entry_bounding.unionWith( rtreenode->children_leaves[ i ].bounding_box );
        }
    } 
    return RTreeRecord_NonLeaf( new_entry_bounding, rtreenode );
}



/*
    print the elements of the tree
    based on ordered tree walking 
*/
void RTree::print_tree(RTreeNode* subtree_root, int depth ) const{

    if( subtree_root->children_nodes.size() > 0 ){ 

        // descend in each one of the elements and call print_tree
        for( unsigned i=0; i < subtree_root->children_nodes.size(); i++ ){
            //print spaces for indentation
            for(int j=0; j < depth; j++){
                std::cout << "  " ;
            }

            std::cout << subtree_root->children_nodes[i].bounding_box << ",  " << subtree_root->children_nodes.size() << std::endl ;
            _RTREE_PRINT_TREE_INS( subtree_root->children_nodes[i].data, depth+1, used_during_insert);
        }

    }
    else{   
       for(int j=0; j < depth; j++){
            std::cout << "  " ;
        }
        std::cout << subtree_root->children_leaves.size() << ": " ;

        // print all the elements of the leaf node
        for( unsigned i=0; i < subtree_root->children_leaves.size(); i++ ){
            std::cout << subtree_root->children_leaves[i].data << ", " ;
        }
        std::cout << std::endl ;

    }
}


void RTree::sanity_check(RTreeNode* subtree_root, int depth, bool used_during_insert  ) const{

    if( subtree_root->children_nodes.size() > 0 ){ 
        // descend in each one of the elements and call sanity_check
        for( unsigned i=0; i < subtree_root->children_nodes.size(); i++ ){
            sanity_check( subtree_root->children_nodes[i].data, depth+1, used_during_insert);
        }


        // sanity check
        if( subtree_root != root ){
            assert( subtree_root->children_nodes.size() >= min_records);
        }
/*
        else{
            assert( subtree_root->children_nodes.size() >= 1);
        }
*/

        if( used_during_insert ){
            // allow one more during for insert
            assert( subtree_root->children_nodes.size() <= max_records + 1 );
        }
        else{
            assert( subtree_root->children_nodes.size() <= max_records );
        }

    }
    else{   
        // sanity check
        if( subtree_root != root ){
            assert( subtree_root->children_leaves.size() >= min_records);
        }
/*
        else{
            assert( subtree_root->children_leaves.size() >= 1);
        }
*/

        if( used_during_insert ){
            // allow one more during for insert
            assert( subtree_root->children_leaves.size() <= max_records + 1 ); 
        }
        else{
            assert( subtree_root->children_nodes.size() <= max_records );
        }
    }
}



/*=============================================================================
                                search
===============================================================================
Given an RTree whose root node is T find all index records whose rects overlap search rect S
S1) Search subtrees:
    IF T isnt a leaf, check every entry E to determine whether E I overlaps S
        FOR all overlapping entries invoke Search on the tree whose root node is pointed by E P
S2) ELSE T is leaf
        check all entries E to determine whether E I overlaps S
        IF so E is a qualifying record
*/


void RTree::search( const Rect &search_area, std::vector< int >* result, const RTreeNode* subtree ) const {
    // S1
    if( subtree->children_nodes.size() > 0  ){   // non-leaf: subtree
        for( unsigned i = 0; i < subtree->children_nodes.size(); i++  ){
            if( subtree->children_nodes[ i ].bounding_box.intersects( search_area ) ){
                search( search_area, result, subtree->children_nodes[ i ].data );
            }
        }
    }
    // S2
    else{   // leaf: subtree
        for( unsigned i = 0; i < subtree->children_leaves.size(); i++  ){
            if( subtree->children_leaves[ i ].bounding_box.intersects( search_area ) ){
                result->push_back( subtree->children_leaves[ i ].data );
            }
        }
    }    
}


/*=============================================================================
                                  erase
===============================================================================
we changed steps D2)
D1) Find node containing record
    Invoke find_leaf() to locate the leaf node L containing E
    IF record is found stop
D2) Delete record
    Remove E from L (it happened in find_leaf step FL2)
D3) Propagate changes
    Invoke condense_tree, passing L
D4) Shorten tree
    If root node has only one child, after the tree was adjusted, make the child new root

return
0 on success
1 in case no entry was found

*/
//int RTree::erase( const RTreeRecord_Leaf & record_to_erase ){
int RTree::erase( const Rect &search_area, const int shape_to_delete ){
    _RTREE_PRINT("\n=====================================");
    _RTREE_PRINT("erase element: " << shape_to_delete);
    // D1 + D2: entry is deleted in find_leaf
    _RTREE_PRINT("D1 & D2 : find and delete the leaf");
    RTreeNode* contains_record = find_leaf( root, search_area, shape_to_delete );
    if( !contains_record ){ // no entry returned from find_leaf
        return 1; // no entry found
    }
    
    // D3
    //bool root_elimination_performed = condense_tree( contains_record );

    // D4

    //if( root_elimination_performed ){
    if( root->children_nodes.size() > 0 ){ // non leaf: root
        // D4
        if( root->children_nodes.size() == 1 ){
            _RTREE_PRINT("D4 : non leaf: ");
            tree_height--;
            RTreeNode* t = root;
            root = root->children_nodes[0].data;
            delete t;
        }

    }
    else { // leaf: root
        // D4
        // do nothing
    }
    sanity_check( root, 0 );
    return 0; // success
}


/*
    find_leaf()
Given an RTree whose root node is T find the leaf node containing index entry E

FL1) Search subtrees
    IF T is non leaf, check each entry F in T to determine if F I overlaps E I 
        foreach such entry invoke find_leaf on the tree whose root is pointed to by F P until E is 
        found or all entries have been checked
FL2) search leaf node for record
    IF T is leaf, check each entry to see if it matches E
        IF E is found return T 
        AND delete element E (step D2)
*/

RTreeNode* RTree::find_leaf( RTreeNode* subtree, const Rect &search_area, const int shape_to_delete ) const {
    // FL1
    if( subtree->children_nodes.size() > 0  ){   // non-leaf: subtree
        for( std::vector< RTreeRecord_NonLeaf >::iterator it = subtree->children_nodes.begin(); it!=subtree->children_nodes.end(); ++it ){
            if( it->bounding_box.intersects( search_area ) ){
                RTreeNode* t = find_leaf( it->data, search_area, shape_to_delete );
                if( t ){ // if search was successful terminate
                    return t;
                }
            }
        }
    }
    // FL2
    else{   // leaf: subtree
        for( std::vector< RTreeRecord_Leaf >::iterator it = subtree->children_leaves.begin(); it!=subtree->children_leaves.end(); ++it ){
            if( it->data == shape_to_delete ){
                // delete element: implement step D2)
                subtree->children_leaves.erase( it ); 
                return subtree;
            }
        }
    }
    return 0;
}


/*
    condense_tree()
Given a Leaf node L from which an entry has been deleted eliminate the node if it has too few entries and reallocate its entries
Propagate node elimination upwards as necessary
Adjust all covering recsts n the path to the root making them smaller if possible

CT1) Initialize
    Set N=L 
    Set Q the set of eliminated nodes to be empty
CT2) // Find parent entry (was there but doesn't make sense)
    IF N is the root 
        goto CT6
    ELSE 
        1) Find parent entry
        2) let P be the parent of N 
        3) and let EN be N's entry in P
CT3) IF N has fewer than m entries 
        Eliminate underfull node
        1) delete EN from P 
        2) and add N to set Q
CT4) ELSE 
        adjust EN I to tightly contain all entries in N
CT5) move up one level in tree
    set N=P and repeat from CT2

CT6) Re insert orphaned entries
    Re-inser all entreis of nodes in set Q
    Entries from eliminated leaf nodes are re-inserted in tree leaves (like in insert)
    BUT non-leaf nodes must be placed higher in the tree so that leaves of their dependent subtrees
    will be on the same level as leaves of the main tree. (on the same height they originally were)
        (not mentioned in the source description: the criteria for placing the node should be 
        again TODO ??? least enlargement)

*/
// TODO this can be merged with adjust_tree or refactor to reutilize some parts. less readable
bool RTree::condense_tree( RTreeNode* position )
{
    RTreeNode* parent;
    unsigned child_in_parent = 0; // the element in parent node that points to current posistion

    std::pair< RTreeNode*, bool > find_result;
    bool elimination_performed = false;
    bool root_elimination_performed = false;
    unsigned current_height = tree_height+1;
    Rect special_case_bounding_box;
    _RTREE_PRINT("  condense_tree");   
    _RTREE_PRINT("  CT1");
    // leaf records that were eliminated due to under-full node
    std::vector< RTreeRecord_Leaf > Q_leaf_records( 0 );

    // < non-leaf records, their height > that were eliminated due to under-full node
    std::vector< std::pair< RTreeRecord_NonLeaf, unsigned > > Q_nonleaf_records( 0 );


    while( true ){

        // check for loop BREAK
        if( position == root ){
            _RTREE_PRINT("  CT2   position is root");
            if( elimination_performed ){
                root_elimination_performed = true;
            }            
            break;
        }

        /* 
            pick randomly, let's say the 1st entry of the current node. 
            Search for this spatial area in the tree, and stop to the parent node.
            Then find position of current node pointer, in the parent node.
        */
        /* 
            special case. if elimination due to children being underfull was performed AND
            AND parent had only 1 record ,then this one record was removed.
        */
        if( position->children_nodes.size() > 0 ){
            _RTREE_PRINT("  CT2.1 - 2   non leaf: find parent, P is parent");    
            // CT2.1   find parent. By definition it's nonleaf
            find_result = find_parent( root, position->children_nodes[0].bounding_box, position);
        }
        else if( position->children_nodes.size() == 0 
            && position->children_leaves.size() == 0 
            && elimination_performed )
        { // special case
            _RTREE_PRINT("  CT2.1 - 2   special case: find parent, P is parent");    
            // CT2.1   find parent. By definition it's nonleaf
            find_result = find_parent( root, special_case_bounding_box, position);
        }
        else{
            _RTREE_PRINT("  CT2.1 - 2   leaf: find parent, P is parent");    
            // CT2.1   find parent. By definition it's nonleaf
            find_result = find_parent( root, position->children_leaves[0].bounding_box, position);
        }
        // CT2.2 Let P be the parent of N
        parent = find_result.first;

        
        // parent is a non-leaf, by definition. Calculate "child_in_parent"
        _RTREE_PRINT("  CT2.3   find in parent, position's record EN");    
        // CT2.3    Let EN be the N's entry in P
        for( child_in_parent = 0; child_in_parent < parent->children_nodes.size(); child_in_parent++ ){
            if( parent->children_nodes[ child_in_parent ].data == position){
                _RTREE_PRINT("  child_in_parent: " << child_in_parent << " out of " << parent->children_nodes.size() << " (size)" );
                break;
            }
        }

        if( position->children_nodes.size() > 0 ){ // non leaf node: position
            _RTREE_PRINT("  CT3   nonleaf: eliminate underfull node");    
            // CT3 Eliminate underfull node
            if( position->children_nodes.size() < min_records ){  
                _RTREE_PRINT("  CT3.2   add N to Q");    
                // CT3.2 add N to set Q ( EN the record that points to N )
                for( unsigned i = 0; i < position->children_nodes.size(); i++ ){
                    _RTREE_PRINT("  i " << i );
                    std::pair< RTreeRecord_NonLeaf, unsigned > t = std::make_pair( position->children_nodes[i], current_height-1);
                    Q_nonleaf_records.push_back( t );

                }
                special_case_bounding_box = parent->children_nodes[ child_in_parent ].bounding_box;

                _RTREE_PRINT("  CT3.1   delete in parent, position's record EN");    
                // CT3.1 delete EN from P  ( parent is by definition nonleaf )
                if( remove_record_from_parent( parent, position ) ){ // TODO does erase, delete to the pointer ???
                    _RTREE_PRINT("  remove_record_from_parent error ");
                }
                elimination_performed = true;
            }
            else{   
                _RTREE_PRINT("  CT4 ");    /// CT4) if not underfull
                recalculate_bounding_box( parent, position, child_in_parent );
                elimination_performed = false;
            }

        }
        else{ // leaf node: position
            _RTREE_PRINT("  CT3   leaf: eliminate underfull node");    
            // CT3 Eliminate underfull node
            if( position->children_leaves.size() < min_records ){  
                _RTREE_PRINT("  CT3.2   add N to Q " << position->children_leaves.size() );    
                // CT3.2 add N to set Q
                for( unsigned i = 0; i < position->children_leaves.size(); i++ ){
                    _RTREE_PRINT("  i " << i );
                    Q_leaf_records.push_back( position->children_leaves[i] ); // TODO problem here
                    special_case_bounding_box = position->children_leaves[i].bounding_box;
                }

                _RTREE_PRINT("  CT3.1   delete in parent, position's record EN");    
                // CT3.1 delete EN from P ( parent is by definition nonleaf )
                if( remove_record_from_parent( parent, position ) ){
                    _RTREE_PRINT("  remove_record_from_parent error ");
                }
                elimination_performed = true;
            }
            else{   
                _RTREE_PRINT("  CT4 ");    /// CT4) if not underfull
                recalculate_bounding_box( parent, position, child_in_parent );
                elimination_performed = false;
            }
        }
        _RTREE_PRINT("  CT5 ");// CT5) move up one level in tree
        position = parent;

        current_height--;
    }
    // CT6: reinsert
    _RTREE_PRINT("  ------ Q_leaf");
    for( std::vector< RTreeRecord_Leaf >::iterator it = Q_leaf_records.begin(); it != Q_leaf_records.end(); ++it ){
        _RTREE_PRINT("  leaf:" << (*it).data);
    }
     _RTREE_PRINT("  ------ Q_nonleaf");
    for( std::vector< std::pair< RTreeRecord_NonLeaf, unsigned > >::iterator it = Q_nonleaf_records.begin(); it != Q_nonleaf_records.end(); ++it ){
        _RTREE_PRINT(" ------- " << it->second );
        _RTREE_PRINT_TREE( it->first.data, 0);
    }

    _RTREE_PRINT("  CT6 ");
    for( std::vector< RTreeRecord_Leaf >::iterator it = Q_leaf_records.begin(); it != Q_leaf_records.end(); ++it ){
        insert( *it );
        _RTREE_PRINT("  inserted leaf:" << (*it).data << "  ------------");
        _RTREE_PRINT_TREE( root, 0);
    }

    
    for( std::vector< std::pair< RTreeRecord_NonLeaf, unsigned > >::iterator it = Q_nonleaf_records.begin(); it != Q_nonleaf_records.end(); ++it ){
        insert( RTreeRecord_Leaf() , true, it->second, it->first );
        _RTREE_PRINT("  inserted nonleaf------------");
        _RTREE_PRINT_TREE( root, 0);
        // TODO this fake RTreeRecord_Leaf() looks stupid. find better way to to this ???
    }
    
    return root_elimination_performed;
}


/*
given:
- a parent
- a child node
- and the position of the child node in the parent
recalculate the parent record's bounding box of the child, in order to ightly contain all entries of child

NOTE! child must be indeed child of the parent, otherwise it screws up things. So find parent and child
before calling this function
*/
void RTree::recalculate_bounding_box( RTreeNode* parent, RTreeNode* child, unsigned &child_in_parent ) {
    if( child->children_nodes.size() > 0 ){
        _RTREE_PRINT("  non-leaf: recalculate bounding box of parent "); // non leaf-node: child
        parent->children_nodes[ child_in_parent ].bounding_box = Rect( child->children_nodes[0].bounding_box );
        for( unsigned i=1; i < child->children_nodes.size(); i++ ){
            parent->children_nodes[ child_in_parent ].bounding_box.unionWith( child->children_nodes[i].bounding_box );
        }
    }
    else{ 
        _RTREE_PRINT("  leaf: recalculate bounding box of parent ");    // leaf-node: child
        parent->children_nodes[ child_in_parent ].bounding_box = Rect( child->children_leaves[0].bounding_box );

        for( unsigned i=1; i < child->children_leaves.size(); i++ ){
            parent->children_nodes[ child_in_parent ].bounding_box.unionWith( child->children_leaves[i].bounding_box );
        }
    }
}

/*
given:
- a parent
- a child node
it removes the child record from the parent

NOTE! child must be indeed child of the parent, otherwise it screws up things. 
So find parent and child before calling this function
*/
int RTree::remove_record_from_parent( RTreeNode* parent, RTreeNode* child ) {
    _RTREE_PRINT( "remove_record_from_parent)" );
    for( std::vector< RTreeRecord_NonLeaf >::iterator it = parent->children_nodes.begin(); it!=parent->children_nodes.end(); ++it ){
        if( it->data == child ){
            // delete element: implement step D2)
            parent->children_nodes.erase( it ); 
            return 0; // sucess
        }
    }
    return 1; // failure
}

/*=============================================================================
TODO                            update
===============================================================================
*/


};

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
