/* -*- C++ -*- */

#include <stddef.h>
#include <assert.h>

template<typename T> class RandomAccessList {
 public:
  RandomAccessList() : chain(NULL) {
  }

  T lookup(size_t i) const {
    return lookup_list(chain, i);
  }

  const RandomAccessList<T> *update(size_t i, T y) const {
    return new RandomAccessList(update_list(chain, i, y));
  }

  bool is_empty() const {
    return chain == NULL;
  }

  const RandomAccessList<T> *cons(T x) const {
    return new RandomAccessList<T>(cons_list(x, chain));
  }

  T head() const {
    return head_list(chain);
  }

  const RandomAccessList<T> *tail() const {
    return new RandomAccessList(tail_list(chain));
  }

 private:
  class Tree {
   public:
    explicit Tree(T value) : x(value), left(NULL), right(NULL) {
    }

    Tree(T value, const Tree *ltr, const Tree *rtr)
        : x(value), left(ltr), right(rtr) {
    }

    T lookup(size_t size, size_t i) const {
      if (i == 0) {
        return x;
      } else {
        assert(!is_leaf());
        size_t s = size/2;
        if (i <= s) {
          return left->lookup(s, i-1);
        } else {
          return right->lookup(s, i-1-s);
        }
      }
    }

    const Tree *update(size_t size, size_t i, T y) const {
      if (i == 0) {
        if (is_leaf()) {
          return new Tree(y);
        } else {
          return new Tree(y, left, right);
        }
      } else {
        assert(!is_leaf());
        size_t s = size/2;
        if (i <= s) {
          return new Tree(x, left->update(s, i-1, y), right);
        } else {
          return new Tree(x, left, right->update(s, i-1-s, y));
        }
      }
    }

    bool is_leaf() const {
      return left == NULL; // && right == NULL
    }

   private:
    const T x;
    const Tree *left;
    const Tree *right;
  };

  class List {
   public:
    List(size_t s, const Tree *tr, const List *rst)
        : size(s), tree(tr), rest(rst) {
    }

    const size_t size;
    const Tree *tree;
    const List *rest;
  };

  explicit RandomAccessList(const List *list) : chain(list) {
  }

  T lookup_list(const List *list, size_t i) const {
    assert(list != NULL);

    if (i < list->size) {
      return list->tree->lookup(list->size, i);
    } else {
      return lookup_list(list->rest, i-list->size);
    }
  }

  const List *update_list(const List *list, size_t i, T y) const {
    assert(list != NULL);

    if (i < list->size) {
      return new List(list->size, list->tree->update(list->size, i, y), list->rest);
    } else {
      return new List(list->size, list->tree, update_list(list->rest, i-list->size, y));
    }
  }

  const List *cons_list(T x, const List *list) const {
    if (list != NULL && list->rest != NULL) {
      size_t s1 = list->size, s2 = list->rest->size;
      if (s1 == s2) {
        return new List(1+s1+s2,
                        new Tree(x, list->tree, list->rest->tree),
                        list->rest->rest);
      } else {
        return new List(1, new Tree(x), list);        
      }
    } else {
      return new List(1, new Tree(x), list);
    }
  }

  T head_list(const List *list) const {
    assert(list != NULL);

    return list->tree->x;
  }

  const List *tail_list(const List *list) const {
    assert(list != NULL);

    if (list->tree->is_leaf()) {
      return list->rest;
    } else {
      size_t s = list->size/2;
      return new List(s, list->tree->left,
                      new List(s, list->tree->right,
                               list->rest));
    }
  }

  const List *chain;
};
