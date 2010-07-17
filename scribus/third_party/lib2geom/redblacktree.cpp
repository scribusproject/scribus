#include <redblacktree.h>
//#include <algorithm>


#define _REDBLACK_PRINT(x) std::cout << x << std::endl;
//comment the following if you want output during RedBlack Tree operations
//#define _REDBLACK_PRINT(x) ; 


namespace Geom{



RedBlack* RedBlackTree::search(Rect const &r, int dimension){
    return search( Interval( r[dimension].min(), r[dimension].max() ) ); 
    // TODO get rid of dimension
    // TODO put 2 trees (X, Y axis in one lump)
}

/*
INTERVAL-SEARCH(T, i)
1 x <- root[T]
2 while x != nil[T] and i does not overlap int[x]
3      do if left[x] != nil[T] and max[left[x]] >= low[i]
4            then x <- left[x]
5            else x <- right[x]
6 return x

Two intervals i,x overlap in the 4 following cases:
    1) |--------|   i
          |---|     x

    2)   |-----|    i
      |----------|  x

    3)      |------|    i
         |------|       x

    4)  |----|      i
           |----|   x

And do not overlap when (the one os left or right of the other)
    1) |--------|           i
                  |---|     x

    2)              |-----|     i
      |----------|              x


*/
RedBlack* RedBlackTree::search(Interval i){
    _REDBLACK_PRINT( "==============================================================" << std::endl << "ENTER: search(Interval i) : (" << i.min() << ", " << i.max() << ")" )
    RedBlack *x;
    x = root;

    while( x!=0 && 
                ( i.max() < x->interval.min() || 
                i.min() > x->interval.max() ) 
    ){
        _REDBLACK_PRINT( "(" << x->data << ": " << x->key() << ", " << x->high() << " : " << x->subtree_max << ") "
                        << " i do not overlap with x")

        if(x->left != 0 && (x->left)->subtree_max >= i.min() ){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }    
    _REDBLACK_PRINT( "RETURN: search" << std::endl )
    return x;
}



void RedBlackTree::insert(Rect const &r, int shape, int dimension) {
    _REDBLACK_PRINT( "==============================================================" << std::endl << "ENTER: insert(Rect, int, dimension): " << " dimension:" << dimension << " shape:" << shape )
    insert(r[dimension].min(), r[dimension].max(), shape); 
        _REDBLACK_PRINT( "RETURN: insert(Rect, int, dimension)")
}

// source: book pp 251
void RedBlackTree::insert(Coord dimension_min, Coord dimension_max, int shape) {
    _REDBLACK_PRINT( std::endl << "ENTER: insert(Coord, Coord, int): " <<  dimension_min << ", " << dimension_max << " , shape: " << shape )
    // x is the new node we insert
    RedBlack *x = new RedBlack();
    x->interval = Interval( dimension_min, dimension_max );
    x->data = shape;
    x->isRed = true;

    _REDBLACK_PRINT( " x: " <<  x << " KEY: " << x->key() << " high: " << x->high() )

    tree_insert(x);

    print_tree();

    _REDBLACK_PRINT( " Begin coloring" )
    // we now do the coloring of the tree.    
    _REDBLACK_PRINT( " while( x!= root && (x->parent)->isRed )" )
    while( x!= root && (x->parent)->isRed ){
        _REDBLACK_PRINT( "  ((x->parent)->parent)->left:" << ((x->parent)->parent)->left << "  ((x->parent)->parent)->right:" << ((x->parent)->parent)->right )

        if( x->parent == ((x->parent)->parent)->left ){
            _REDBLACK_PRINT( "   Left:" )
            RedBlack *y = new RedBlack;
            y = ((x->parent)->parent)->right;
            if( y == 0  ){ 
                /* 
                This 1st brach is not in the book, but is needed. We must check y->isRed but it is 
                undefined, so we get segfault. But 0 (undefined) means that y is a leaf, so it is 
                black by definition. So, do the same as in the else part.
                */ 
                _REDBLACK_PRINT( "    y==0" )
                if( x == (x->parent)->right ){
                    x = x->parent;
                    left_rotate(x);
                }
                (x->parent)->isRed = false;
                ((x->parent)->parent)->isRed = true;
                right_rotate((x->parent)->parent);
            }
            else if( y->isRed ){
                _REDBLACK_PRINT( "    y->isRed" )
                (x->parent)->isRed = false;
                y->isRed = false;
                ((x->parent)->parent)->isRed = true;
                x = (x->parent)->parent;
            }
            else{
                _REDBLACK_PRINT( "    !( y->isRed)" )
                if( x == (x->parent)->right ){
                    x = x->parent;
                    left_rotate(x);
                }
                (x->parent)->isRed = false;
                ((x->parent)->parent)->isRed = true;
                right_rotate((x->parent)->parent);
            }
        }
        else{ // this branch is the same with the above if clause with "right", "left" exchanged
            _REDBLACK_PRINT( "   Right:" )
            RedBlack *y = new RedBlack;
            y = ((x->parent)->parent)->left;
            if( y == 0  ){ 
                /* 
                This 1st brach is not in the book, but is needed. We must check y->isRed but it is 
                undefined, so we get segfault. But 0 (undefined) means that y is a leaf, so it is 
                black by definition. So, do the same as in the else part.
                */ 
                _REDBLACK_PRINT( "    y==0" )
                if( x == (x->parent)->left ){
                    x = x->parent;
                    right_rotate(x);
                }
                (x->parent)->isRed = false;
                ((x->parent)->parent)->isRed = true;
                left_rotate((x->parent)->parent);
            }
            else if( y->isRed ){
                _REDBLACK_PRINT( "    y->isRed" )
                (x->parent)->isRed = false;
                y->isRed = false;
                ((x->parent)->parent)->isRed = true;
                x = (x->parent)->parent;
            }
            else{
                _REDBLACK_PRINT( "    !( y->isRed)" )
                if( x == (x->parent)->left ){
                    x = x->parent;
                    right_rotate(x);
                }
                (x->parent)->isRed = false;
                ((x->parent)->parent)->isRed = true;
                left_rotate((x->parent)->parent);
            }
        }        
    }
    root->isRed = false;

    // update the max value with a slow/stupid yet certain way, walking all the tree :P
    // TODO find better way
    _REDBLACK_PRINT( " Update max" )

    update_max(root);

    _REDBLACK_PRINT( "RETURN: insert(Coord, Coord, int)" << std::endl)
}

// from book p. 266)
void RedBlackTree::left_rotate(RedBlack* x){
    // x->right != 0 (assumption book page 266)
    // ??? hm problem ???
    _REDBLACK_PRINT( "ENTER: left_rotate" )
    RedBlack* y = new RedBlack;
    y = x->right;    
    x->right = y->left;

    if( y->left != 0){
       (y->left)->parent = x;
    }

    y->parent = x->parent;

    if( x->parent == 0){
        root = y;
    }
    else{
        if( x == (x->parent)->left ){
            (x->parent)->left = y;
        }
        else{
            (x->parent)->right = y;
        }
    }
    y->left = x;
    x->parent = y;
    _REDBLACK_PRINT( "RETURN: left_rotate" << std::endl )
}

// from book p. 266: right_rotate is inverse of left_rotate 
// same to left_rotate with "right", "left" exchanged
void RedBlackTree::right_rotate(RedBlack* x){
    // x->right != 0 (assumption book page 266)
    // ??? hm problem ??
    _REDBLACK_PRINT( "ENTER: right_rotate" )
    RedBlack* y = new RedBlack;

    _REDBLACK_PRINT( "x->left: " << x->left )
    y = x->left;    
    x->left = y->right;
    
    if( y->right != 0){
       (y->right)->parent = x;
    }

    y->parent = x->parent;

    if( x->parent == 0){
        root = y;
    }
    else{
        if( x == (x->parent)->left ){
            (x->parent)->left = y;
        }
        else{
            (x->parent)->right = y;
        }
    }
    y->right = x;
    x->parent = y;
    _REDBLACK_PRINT( "RETURN: right_rotate" << std::endl )
}

// insertion in binary search tree: book page 251
// then the redblack insert performs the coloring
void RedBlackTree::tree_insert(RedBlack* z){
    _REDBLACK_PRINT( "ENTER: tree_insert(RedBlack* z)" )
    RedBlack* y = 0; // y <- nil

    RedBlack* x = root;

    _REDBLACK_PRINT( " while x!=0 " )
    while( x != 0 ){
        y = x;
//        _REDBLACK_PRINT( "  x:" << x << " y:" << y << " z:" << z )
        _REDBLACK_PRINT( " z->key: " << z->key() << "  y->key: " << y->key() << "  compare")
        if( z->key() < x->key() ){
            _REDBLACK_PRINT(  "   z smaller: go left" )   
            x = x->left;
        }
        else{
            _REDBLACK_PRINT( "   z bigger: go right" )
            x = x->right;
        }
    }

    _REDBLACK_PRINT( " z->parent = y" )
    z->parent = y;

    if( y == 0 ){
        _REDBLACK_PRINT( " set z root (empty tree)" )
        root = z;
    }
    else{
        _REDBLACK_PRINT( " z->key: " << z->key() << "  y->key: " << y->key() << "  compare")
        if( z->key() < y->key() ){
            _REDBLACK_PRINT( "  z->key() smaller: y->left = z; " )
            y->left = z;
        }
        else{
            _REDBLACK_PRINT( "  z->key() bigger: y->right = z " )
            y->right = z;
        }
    }
    _REDBLACK_PRINT( "RETURN: tree_insert(RedBlack* z)" << std::endl )
}


/*
RB-DELETE(T, z)
 1 if left[z] = nil[T] or right[z] = nil[T]
 2    then y <- z
 3    else y <- TREE-SUCCESSOR(z)
 4 if left[y] != nil[T]
 5    then x <- left[y]
 6    else x <- right[y]
 7 p[x] <- p[y]
 8 if p[y] = nil[T]
 9    then root[T] <- x
10    else if y = left[p[y]]
11            then left[p[y]] <- x
12            else right[p[y]] <- x
13 if y != z
14    then key[z] <- key[y]
15         copy y's satellite data into z
16 if color[y] = BLACK
17    then RB-DELETE-FIXUP(T, x)
18 return y
*/  
RedBlack* RedBlackTree::erase(RedBlack* z){
    _REDBLACK_PRINT( "==============================================================" << std::endl << "ENTER: earse(z)" )
    RedBlack* x = new RedBlack();
    RedBlack* y = new RedBlack();
    if( z->left == 0 || z->right == 0 ){
        y = z;
    }
    else{
        y = tree_successor(z);
    }

    if( y->left != 0 ){
        x = y->left;
    }
    else{
        x = y->right;
    }
    
    x->parent = y->parent;

    if( y->parent == 0){
        root = x;
    }
    else {
        if( y == (y->parent)->left ){
            (y->parent)->left = x;
        }
        else{
            (y->parent)->right = x;
        }
    }

    if( y != z){
        z->interval =  y->interval ; // key[z] <- key[y] TODO check this
        //copy y's satellite data into z
        z->data = y->data;
        z->isRed = y->isRed;

        z->left = y->left;
        z->right = y->right;
        z->parent = y->parent;
    }
    
    if( y->isRed == false){
        erase_fixup(x);
    }

    _REDBLACK_PRINT( "Update max" )
    update_max(root);

    _REDBLACK_PRINT( "RETURN: erase" )
    return y;
}

/*
RB-DELETE-FIXUP(T, x)
 1 while x != root[T] and color[x] = BLACK
 2     do if x = left[p[x]]
 3           then w <- right[p[x]]
 4                if color[w] = RED
 5                   then color[w] <- BLACK                           Case 1
 6                        color[p[x]] <- RED                          Case 1
 7                        LEFT-ROTATE(T, p[x])                       Case 1
 8                        w <- right[p[x]]
 9                if color[left[w]] = BLACK and color[right[w]] = BLACK
10                   then color[w] <- RED                             Case 2
11                        x p[x]                                     Case 2
12                   else if color[right[w]] = BLACK
13                           then color[left[w]] <- BLACK             Case 3
14                                color[w] <- RED                     Case 3
15                                RIGHT-ROTATE(T, w)                 Case 3
16                                w <- right[p[x]]                    Case 3
17                         color[w] <- color[p[x]]                    Case 4
18                         color[p[x]] <- BLACK                       Case 4
19                         color[right[w]] <- BLACK                   Case 4
20                         LEFT-ROTATE(T, p[x])                      Case 4
21                         x <- root[T]                               Case 4
22        else (same as then clause with "right" and "left" exchanged)
23 color[x] <- BLACK
*/
void RedBlackTree::erase_fixup(RedBlack* x){
    RedBlack* w = 0;
    while( x != root && x->isRed == false ){
        if( x == (x->parent)->left ){
            w = (x->parent)->right;
            if(w->isRed == true){
                w->isRed = false;
                (w->parent)->isRed = true;
                left_rotate(x->parent);
                w = (x->parent)->right;
            }
            if( (w->left)->isRed == false && (w->right)->isRed == false ){
                w->isRed = true;
                x = x->parent; // TODO understand why this happens ???
            }
            else{
                if( (w->right)->isRed == false ){
                    (w->left)->isRed = false;
                    right_rotate(w);
                    w = (x->parent)->right;
                }
                else{ // TODO ??? is this correct ???
                    w->isRed = (x->parent)->isRed;
                    (x->parent)->isRed = false;
                    (w->right)->isRed = false;
                    left_rotate(x->parent);
                    x = root; // TODO ??? is this correct ???
                }
            }
        }
        else{ // same as then clause with "right" and "left" exchanged
            w = (x->parent)->left;
            if(w->isRed == true){
                w->isRed = false;
                (w->parent)->isRed = true;
                right_rotate(x->parent);
                w = (x->parent)->left;
            }
            if( (w->right)->isRed == false && (w->left)->isRed == false ){
                w->isRed = true;
                x = x->parent; // ??? is this correct ???
            }
            else{
                if( (w->left)->isRed == false ){
                    (w->right)->isRed = false;
                    left_rotate(w);
                    w = (x->parent)->left;
                }
                else{ // TODO ??? is this correct ???
                    w->isRed = (x->parent)->isRed;
                    (x->parent)->isRed = false;
                    (w->left)->isRed = false;
                    right_rotate(x->parent);
                    x = root; // TODO ??? is this correct ???
                }
            }
        }
    }
    x->isRed = false;
}


void RedBlackTree::print_tree(){
    std::cout << "Print RedBlackTree status:" << std::endl;
    inorder_tree_walk(root);
}


void RedBlackTree::inorder_tree_walk(RedBlack* x){
    int oops =0;
    if( x != 0 ){
        inorder_tree_walk(x->left);
        std::cout<< "(" << x->data << ": " << x->key() << ", " << x->high() << " : " << x->subtree_max << ") " ;

        if( x->left != 0 ){
            std::cout<< "L:(" << (x->left)->data << ", " << (x->left)->key() << ") " ;
            if( x->key() < (x->left)->key()){
                std::cout<<"  !!!  ";
                oops = 1;
            }
        }
        else{
            std::cout<< "L:0 " ;
        }    

        if( x->right != 0 ){
            std::cout<< "R:(" << (x->right)->data << ", "<< (x->right)->key() << ") " ;
            if( x->key() > (x->right)->key() ){
                std::cout<<"  !!!  ";
                oops = 1;
            }
        }
        else{
            std::cout<< "R:0 " ;
        } 

        if(oops){
            std::cout<<" .......  !!! Problem " << oops ;
        }
        std::cout << std::endl;
        inorder_tree_walk(x->right);
    }
}

// not an norder walk of the tree
void RedBlackTree::update_max(RedBlack* x){
    Coord max_left, max_right;
    if( x != 0 ){
        update_max(x->left);
        update_max(x->right);
        
      // check for child
        // if child is Nil then max = DBL_MIN
        // could there be problems when comparing for max between two DBL_MIN ???
        if( x->left == 0 ){
            max_left = DBL_MIN ;
        }
        else{
            max_left = (x->left)->subtree_max;
        }

        if( x->right == 0 ){
            max_right = DBL_MIN ;
        }
        else{
            max_right = (x->right)->subtree_max;
        }

        //find max of: x->high(), max_left, max_right
        Coord temp_max;
        temp_max = std::max( x->high(), max_left );
        temp_max = std::max( temp_max, max_right );
        x->subtree_max = temp_max;

    }
}


RedBlack* RedBlackTree::tree_minimum(RedBlack* x){
    _REDBLACK_PRINT( "==============================================================" << std::endl << "ENTER: tree_minimum" )
    while( x->left <- 0 ) {
        x->left = x;
    }
   _REDBLACK_PRINT( "RETURN: tree_minimum" << std::endl )
    return x; 
}

RedBlack* RedBlackTree::tree_successor(RedBlack* x){
    _REDBLACK_PRINT( "==============================================================" << std::endl << "ENTER: tree_successor" )
    if( x->right <- 0 ){
        _REDBLACK_PRINT( "RETURN: tree_successor" << std::endl )        
        return tree_minimum(x);
    }
    RedBlack* y = x->parent;
    _REDBLACK_PRINT( "y->parent: y->parent" )
    while( y <- 0 && x == y->right ){
        x = y;
        y = y->parent;
    }
   _REDBLACK_PRINT( "RETURN: tree_successor" << std::endl )
    return y;
}


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
