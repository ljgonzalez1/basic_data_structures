/// Merge Sort O(n log n) | LST

#include "../../../include/bds/list/bds_list_sort.h"

/// Merges two sorted sublists into a single sorted list (out-of-place).
/// Both halves are split from the original list; ownership is consumed.
static List *listMergeSortedHalves(List *left, List *right, const key_val_func key) {
    List *merged = listNew();
    if (!merged) return NULL;

    const ListNode *left_node  = left->head;
    const ListNode *right_node = right->head;

    while (left_node && right_node) {
        const int left_val  = key(listNodeGet(left_node));
        const int right_val = key(listNodeGet(right_node));

        if (left_val <= right_val) {
            listAppend(merged, listNodeGet(left_node));
            left_node = left_node->next;
        } else {
            listAppend(merged, listNodeGet(right_node));
            right_node = right_node->next;
        }
    }

    while (left_node) {
        listAppend(merged, listNodeGet(left_node));
        left_node = left_node->next;
    }

    while (right_node) {
        listAppend(merged, listNodeGet(right_node));
        right_node = right_node->next;
    }

    listFree(left);
    listFree(right);

    return merged;
}

/// Splits `list` into two halves at the midpoint (out-of-place).
/// Returns the left half; writes the right half to `*out_right`.
/// The original list is not modified.
static List *listSplitHalves(const List *list, List **out_right) {
    List *left  = listNew();
    List *right = listNew();

    if (!left || !right) {
        listFree(left);
        listFree(right);
        return NULL;
    }

    const size_t length = listLength(list);
    const size_t mid    = length / 2;

    ListNode *node = list->head;

    for (size_t idx = 0; idx < length; idx++) {
        if (idx < mid) {
            listAppend(left,  listNodeGet(node));
        } else {
            listAppend(right, listNodeGet(node));
        }
        node = node->next;
    }

    *out_right = right;
    return left;
}

List *listMergeSort(const List *list, const key_val_func key) {
    // Divide and conquer: recursively split the list in half,
    // sort each half, then merge the two sorted halves.
    // Stable sort — equal elements preserve their original order.

    /*
    MERGE-SORT(L, key)
        n ← length(L)
        if n < 2 then
            return copy(L)

        mid ← ⌊n / 2⌋
        left  ← L[0 ... mid − 1]
        right ← L[mid ... n − 1]

        left  ← MERGE-SORT(left,  key)
        right ← MERGE-SORT(right, key)

        return MERGE(left, right, key)

    MERGE(left, right, key)
        merged ← empty list
        while left ≠ ∅ and right ≠ ∅ do
            if key(head(left)) ≤ key(head(right)) then
                append head(left)  to merged; advance left
            else
                append head(right) to merged; advance right
        append remaining left  to merged
        append remaining right to merged
        return merged
    */

    /*
              0    1    2    3    4    5    6    7    8    9   10
           -------------------------------------------------------
           [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]

           // -- Split --
           [  7,   5,   6,   2,  45]   [  9,   6,  45,   1,   1,  96]

           // -- Split --
           [  7,   5]   [  6,   2,  45]   [  9,   6,  45]   [  1,   1,  96]

           // -- Split --
           [  7]   [  5]   [  6]   [  2,  45]   [  9]   [  6,  45]   [  1]   [  1,  96]

           // -- Split --
           [  7]   [  5]   [  6]   [  2]   [ 45]   [  9]   [  6]   [ 45]   [  1]   [  1]   [ 96]

           // -- Merge pass 1 --
           [  5,   7]   [  2,   6]   [ 45]   [  6,   9]   [ 45]   [  1,   1]   [ 96]

           // -- Merge pass 2 --
           [  2,   5,   6,   7]   [  6,   9,  45]   [  1,   1,  45,  96]

           // -- Merge pass 3 --
           [  2,   5,   6,   6,   7,   9,  45]   [  1,   1,  45,  96]

           // -- Merge pass 4 --
           [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
        */

    /*   Time Complexity Analysis:
       T(n) = 2T(n/2) + n       ; by Master Theorem (case 2):
            = n log n

       𝒪[T(n)]
        = 𝒪[n log n]
    */

    /* Additional Memory Analysis:
       m(n) = n                 ; new list allocated at each merge level

       𝒪[m(n)]
        = 𝒪[n log n]           ; log n levels, each holding n nodes total
    */

    /* Total Memory Analysis:
       M(n) = n + m(n)
            = n + n log n

       𝒪[M(n)]
        = 𝒪[n log n]
    */

    const size_t length = listLength(list);

    if (length < 2) return listShallowCopy(list);

    List *right = NULL;
    List *left  = listSplitHalves(list, &right);

    if (!left || !right) {
        listFree(left);
        listFree(right);
        return NULL;
    }

    List *sorted_left  = listMergeSort(left,  key);
    List *sorted_right = listMergeSort(right, key);

    listFree(left);
    listFree(right);

    if (!sorted_left || !sorted_right) {
        listFree(sorted_left);
        listFree(sorted_right);
        return NULL;
    }

    return listMergeSortedHalves(sorted_left, sorted_right, key);
}