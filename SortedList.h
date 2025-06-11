#pragma once
#include <iostream>
#include <stdexcept>

namespace mtm {
    template <typename T>
    class SortedList {
        /**
               *
               * the class should support the following public interface:
               * if needed, use =defualt / =delete
               *
               * constructors and destructor:
               * 1. SortedList() - creates an empty list.
               * 2. copy constructor
               * 3. operator= - assignment operator
               * 4. ~SortedList() - destructor
               *
               * iterator:
               * 5. class ConstIterator;
               * 6. begin method
               * 7. end method
               *
               * functions:
               * 8. insert - inserts a new element to the list
               * 9. remove - removes an element from the list
               * 10. length - returns the number of elements in the list
               * 11. filter - returns a new list with elements that satisfy a given condition
               * 12. apply - returns a new list with elements that were modified by an operation
               */
    private:
        struct Node {
            T value;
            Node* next;
            Node(const T& val, Node* nxt = nullptr) : value(val), next(nxt) {}
        };

        Node* head;
        int size;

        void destroyList();
        void copyNodes(const SortedList<T>& other);
        void unionize(const SortedList<T>& other);

    public:
        class ConstIterator;

        SortedList() : head(nullptr), size(0) {}
        SortedList(const SortedList& other);
        SortedList& operator=(const SortedList& other);
        ~SortedList();

        ConstIterator begin() const;
        ConstIterator end() const;

        void insert(const T& value);
        void remove(const ConstIterator& iter);
        int length() const;

        template<class Predicate>
        SortedList filter(Predicate pred) const;

        template<class Operation>
        SortedList apply(Operation op) const;

       
    };

    template <typename T>
    class SortedList<T>::ConstIterator {
        /**
            * the class should support the following public interface:
            * if needed, use =defualt / =delete
            *
            * constructors and destructor:
            * 1. a ctor(or ctors) your implementation needs
            * 2. copy constructor
            * 3. operator= - assignment operator
            * 4. ~ConstIterator() - destructor
            *
            * operators:
            * 5. operator* - returns the element the iterator points to
            * 6. operator++ - advances the iterator to the next element
            * 7. operator!= - returns true if the iterator points to a different element
            *
            */
    private:
        const SortedList* list;
        Node* current;
        ConstIterator(const SortedList* lst, Node* curr);
        friend class SortedList;

    public:

        ConstIterator() = default;
        ConstIterator(const ConstIterator&) = default;
        ConstIterator& operator=(const ConstIterator&) = default;
        ~ConstIterator() = default;

        ConstIterator& operator++();
        const T& operator*() const;
        bool operator!=(const ConstIterator&) const;
    };

    template<typename T>
    SortedList<T>::SortedList(const SortedList& other) : head(nullptr), size(other.size) {
        copyNodes(other);
    }

    template<typename T>
    void SortedList<T>::copyNodes(const SortedList<T>& other) {
        if (!other.head) return;

        try {
            head = new Node(other.head->value);
            Node* curr = head;
            Node* otherCurr = other.head->next;

            while (otherCurr) {
                curr->next = new Node(otherCurr->value);
                curr = curr->next;
                otherCurr = otherCurr->next;
            }
        } catch (const std::bad_alloc&) {
            destroyList();
            throw;
        }
    }

    template<typename T>
    void SortedList<T>::destroyList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    template<typename T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& other) {
        if (this != &other) {
            SortedList temp(other);
            std::swap(head, temp.head);
            size = other.size;
        }
        return *this;
    }

    template<typename T>
    SortedList<T>::~SortedList() {
        destroyList();
    }

    template<typename T>
    void SortedList<T>::insert(const T& value) {
        Node* newNode = new Node(value);

        if (!head || value > head->value) {  // Uses the overloaded operator> to compare Tasks
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next && current->next->value > value) {  // Uses operator> again
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }

    template<typename T>
    void SortedList<T>::remove(const ConstIterator& iter) {
        if (!iter.current) return;

        if (iter.current == head) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return;
        }

        Node* curr = head;
        while (curr->next && curr->next != iter.current) {
            curr = curr->next;
        }

        if (curr->next) {
            Node* temp = curr->next;
            curr->next = temp->next;
            delete temp;
            size--;
        }
    }

    template<typename T>
    int SortedList<T>::length() const {
        return size;
    }

    template<typename T>
    template<class Predicate>
    SortedList<T> SortedList<T>::filter(Predicate pred) const {
        SortedList<T> result;
        for (ConstIterator it = begin(); it != end(); ++it) {
            if (pred(*it)) {
                result.insert(*it);
            }
        }
        return result;
    }

    template<typename T>
    template<class Operation>
    SortedList<T> SortedList<T>::apply(Operation op) const {
        SortedList<T> result;
        for (ConstIterator it = begin(); it != end(); ++it) {
            result.insert(op(*it));
        }
        return result;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(this, head);
    }

    template<typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(this, nullptr);
    }

    template<typename T>
    void SortedList<T>::unionize(const SortedList<T>& other) {
        for (ConstIterator it = other.begin(); it != other.end(); ++it) {
            insert(*it);
        }
    }

    template<typename T>
    SortedList<T>::ConstIterator::ConstIterator(const SortedList* lst, Node* curr)
            : list(lst), current(curr) {}

    template<typename T>
    typename SortedList<T>::ConstIterator& SortedList<T>::ConstIterator::operator++() {
        if (!current) {
            throw std::out_of_range("Iterator out of range");
        }
        current = current->next;
        return *this;
    }

    template<typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if (!current) {
            throw std::out_of_range("Iterator out of range");
        }
        return current->value;
    }

    template<typename T>
    bool SortedList<T>::ConstIterator::operator!=(const ConstIterator& other) const {
        if (list != other.list) {
            throw std::runtime_error("Comparing iterators from different lists");
        }
        return current != other.current;
    }
}
