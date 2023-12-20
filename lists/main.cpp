#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int VENOM_DATA        = -13;

const int AMOUNT_ELEM = 10;

enum ERROR{
    WITHOUT_ERROR,
    ERROR,
};

typedef struct LIST_ELEM List_elem;
struct LIST_ELEM {
    char      life;
    int       data;
    List_elem *next_elem;
}; 

int add_elem_list(List_elem *head, int new_el);
int delete_list(List_elem *head);
int print_list(List_elem *head, size_t amount);

List_elem *merge_sort(List_elem *head, size_t sz_list, int (*compare)(const void*, const void*));
List_elem *merge(List_elem *head1, size_t sz1, List_elem *head2, size_t sz2, int (*compare)(const void*, const void*));
List_elem *partition_list(List_elem *head, size_t id);

int compare(const void *arg1, const void *arg2);

int main()
{
    List_elem *head_list = (List_elem*)calloc(1, sizeof(List_elem));
    head_list->life      = 0;
    head_list->data      = VENOM_DATA;
    head_list->next_elem = NULL;

    for (int i = 0; i < AMOUNT_ELEM; i++)
    {
        int num = rand() % 100;
        add_elem_list(head_list, num);
    }

    print_list(head_list, (size_t)AMOUNT_ELEM);
    
    head_list = merge_sort(head_list, (size_t)AMOUNT_ELEM, compare);
    
    print_list(head_list, AMOUNT_ELEM);
    
    delete_list(head_list);
}

List_elem *merge_sort(List_elem *head, size_t sz_list, int (*compare)(const void*, const void*))
{
    assert(head    != NULL);
    assert(compare != NULL);

    if (sz_list > 1)
    {
        List_elem *head1 = head;
        List_elem *head2 = partition_list(head, sz_list / 2);

        size_t sz2 = sz_list - sz_list / 2;
        size_t sz1 = sz_list - sz2;
    
        List_elem *head_sort1 = merge_sort(head1, sz1, compare);
        List_elem *head_sort2 = merge_sort(head2, sz2, compare);
    
        return merge(head_sort1, sz1, head_sort2, sz2, compare);
    }
    else
    {
        head->next_elem = NULL;
        return head;
    }
}

List_elem *merge(List_elem *head1, size_t sz1, List_elem *head2, size_t sz2, int (*compare)(const void*, const void*))
{
    if ((sz1 > 0) && (sz2 > 0))
    {
        // printf("merge head1 = %d\n", head1->data);
        // printf("merge head2 = %d\n", head2->data);

        if (compare(&head1->data, &head2->data) <= 0 )
        {
            head1->next_elem = merge(head1->next_elem, sz1 - 1, head2, sz2, compare);
            return head1;
        }
        else
        {
            head2->next_elem = merge(head1, sz1, head2->next_elem, sz2 - 1, compare);
            return head2;
        }
    }
    else if (sz1 > 0)
        return head1;
    else
        return head2;
}

List_elem *partition_list(List_elem *head, size_t id)
{
    if (id > 1)
        return partition_list(head->next_elem, id - 1);
    else
        return head->next_elem;
}

int compare(const void *arg1, const void *arg2)
{
    return *(const int*)arg1 - *(const int*)arg2;
}

int add_elem_list(List_elem *head, int new_el)
{
    if (head->life)
    {
        if (head->next_elem != NULL)
            add_elem_list(head->next_elem, new_el);
        else
        {
            head->next_elem = (List_elem*)calloc(1, sizeof(List_elem));
            add_elem_list(head->next_elem, new_el);
        }
    }
    else
    {
        head->life      = 1;
        head->data      = new_el;
        head->next_elem = NULL;
    }
    return WITHOUT_ERROR;
}

int delete_list(List_elem *head)
{
    if (head->next_elem != NULL)
        delete_list(head->next_elem);

    free(head);
    return WITHOUT_ERROR;
}

int print_list(List_elem *head, size_t amount)
{
    if (amount > 0)
    {
        if (head->life)
        {
            printf("<%2d>", head->data);
            print_list(head->next_elem, amount - 1);
        }
    }    
    else
        printf("\n");

    return WITHOUT_ERROR;
}

