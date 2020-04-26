#ifndef __LIST_HEAD_H__
#define __LIST_HEAD_H__
 

 
struct list_head
{
    struct list_head* head;
    struct list_head* next;
    struct list_head* prev;
    void*  data;
};


static void inline init_list_head(struct list_head* list, void* pdata)
{
    list->head = list;
    list->prev = list;
    list->next = list;
    list->data = pdata;
}


static int inline list_empty(struct list_head* list)
{    
    return (list->prev==list && list->next == list) ? 1 : 0;
}



static void inline list_del(struct list_head* list)
{        
    if (!list_empty(list))
    {                    
        list->prev->next = list->next;
        list->next->prev = list->prev;
    
        list->head = list;
        list->prev = list;
        list->next = list;        
    }            
}


static void inline list_add(struct list_head* list, struct list_head* head)
{
    list_del(list);       // remove it from previous List        
        
    list->head          = head;
    list->prev          = head;
    list->next          = head->next;        
        
    list->prev->next    = list;                  
    list->next->prev    = list;        
}



static void inline list_add_tail(struct list_head* list, struct list_head* head)
{
    list_del(list);       // remove it from previous List        
        
    list->head          = head;
    list->prev          = head->prev;
    list->next          = head;        
        
    list->prev->next    = list;                  
    list->next->prev    = list;        
}

static void inline list_move(struct list_head* list, struct list_head* head)
{       
    list_del(list);     
    list_add(list, head);    
}


static void inline list_move_tail(struct list_head* list, struct list_head* head)
{       
    list_del(list);     
    list_add_tail(list, head);    
}


#define list_for_each(cursor, list)             for (cursor = (list)->next; cursor!=(list); cursor = cursor->next)
#define list_for_each_safe(cursor, next, list)  for (cursor = (list)->next, next = cursor->next ; cursor!=(list); cursor = next, next = cursor->next)

#define list_entry(list)                        (list->data)

#endif //__LIST_HEAD_H__
