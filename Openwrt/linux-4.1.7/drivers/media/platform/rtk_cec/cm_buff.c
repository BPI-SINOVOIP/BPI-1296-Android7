/* ------------------------------------------------------------------------- */
/* cm_buff.c  CEC Message Buffer                                             */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2008 Kevin Wang <kevin_wang@realtek.com.tw>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.    
------------------------------------------------------------------------- 
Update List :
------------------------------------------------------------------------- 
    1.0     |   20081208    | Create Phase
------------------------------------------------------------------------- 
    1.1     |   20081211    | Add Spinlock Protection
-------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "cm_buff.h"

MODULE_LICENSE("GPL");


void cmb_queue_head_init(cm_buff_head *head)
{    
    spin_lock_init(&head->lock);
    INIT_LIST_HEAD(&head->list);      
    head->qlen = 0;
}


void cmb_queue_head(cm_buff_head *head, cm_buff* cmb)
{
    unsigned long flags;
    spin_lock_irqsave(&head->lock, flags);
    
    list_add(&cmb->list,&head->list);
    head->qlen++;
    
    spin_unlock_irqrestore(&head->lock, flags);
}


void cmb_queue_tail(cm_buff_head *head, cm_buff* cmb)
{
    unsigned long flags;
    spin_lock_irqsave(&head->lock, flags);
        
    list_add_tail(&cmb->list,&head->list);
    head->qlen++;
    
    spin_unlock_irqrestore(&head->lock, flags);    
}


cm_buff *cmb_dequeue(cm_buff_head *head)
{    
    cm_buff* cmb = NULL; 
    unsigned long flags;
    spin_lock_irqsave(&head->lock, flags);
        
    if (!list_empty(&head->list))
    {
        cmb = list_entry(head->list.next, cm_buff, list);        
        list_del_init(&cmb->list);                    
        head->qlen--;
    }    
    
    spin_unlock_irqrestore(&head->lock, flags);        
    
    return cmb;
}

cm_buff *cmb_dequeue_tail(cm_buff_head *head)
{
    cm_buff* cmb = NULL; 
    unsigned long flags;
    spin_lock_irqsave(&head->lock, flags);
        
    
    if (!list_empty(&head->list))
    {
        cmb = list_entry(head->list.prev, cm_buff, list);        
        list_del_init(&cmb->list);                    
        head->qlen--;
    }
    
    spin_unlock_irqrestore(&head->lock, flags);            
    
    return cmb;
}


unsigned int cmb_queue_len(const cm_buff_head *head)
{
    return head->qlen;
}


void cmb_queue_purge(cm_buff_head *head)
{
    cm_buff *cmb;
    unsigned long flags;
    spin_lock_irqsave(&head->lock, flags);
        
    while ((cmb = cmb_dequeue(head)) != NULL)
        kfree_cmb(cmb);
        
    spin_unlock_irqrestore(&head->lock, flags);                    
}


cm_buff* alloc_cmb(size_t size)
{        
    cm_buff* cmb = (cm_buff*) kmalloc(sizeof(cm_buff) + size, GFP_KERNEL);
    
    if (cmb)
    {
        INIT_LIST_HEAD(&cmb->list);
        init_waitqueue_head(&cmb->wq);    
        cmb->status = 0; 
        cmb->flags  = 0;
        cmb->head   = ((unsigned char*) cmb) + sizeof(cm_buff);
        cmb->data   = cmb->head;
        cmb->tail   = cmb->head;
        cmb->end    = cmb->head + size;
        cmb->len    = 0;    
    }
    
    return cmb;
}


void kfree_cmb(cm_buff* cmb)
{    
    if (cmb)       
        kfree(cmb);
}


#define CHECK_BOUNDRARY(cmb)		do {\
										if (cmb->tail > cmb->end)\
											printk(KERN_WARNING "cmb over panic:  cmb=%p cmb->tail (%p) > cmb->end (%p)\n", cmb, cmb->tail, cmb->end);\
										if (cmb->data < cmb->head)\
											printk(KERN_WARNING "cmb under panic: cmb=%p cmb->data (%p) < cmb->head (%p)\n", cmb, cmb->data, cmb->head);\
									}while(0)

 
int cmb_tailroom(const cm_buff *cmb)
{
    return cmb->end - cmb->tail;
}
 

// add data from tail of buffer
void cmb_reserve(cm_buff* cmb, unsigned int len)
{
    cmb->data += len;
    cmb->tail += len;        
    CHECK_BOUNDRARY(cmb);
}


// add data from tail of buffer
unsigned char* cmb_put(cm_buff* cmb, unsigned int len)
{
    unsigned char* ptr = cmb->tail;            
    cmb->tail += len;
    cmb->len  += len;
    CHECK_BOUNDRARY(cmb);                            
    return ptr;
}

// add data from start of buffer
unsigned char* cmb_push(cm_buff *cmb, unsigned int len)
{    
    cmb->data -= len;
    cmb->len  += len;
    CHECK_BOUNDRARY(cmb);            
    return cmb->data;
}


// remove data from start of buffer
unsigned char* cmb_pull(cm_buff *cmb, unsigned int len)
{
    if (len <= cmb->len)            
    {
        cmb->data += len;
        cmb->len  -= len;
        return cmb->data;    
    }
    return NULL;    
}
 


void cmb_purge(cm_buff* cmb)
{
    cmb->data = cmb->head;
    cmb->tail = cmb->head;    
    cmb->len  = 0;    
}

EXPORT_SYMBOL(cmb_queue_head_init);
EXPORT_SYMBOL(cmb_queue_head);
EXPORT_SYMBOL(cmb_queue_tail);
EXPORT_SYMBOL(cmb_dequeue);
EXPORT_SYMBOL(cmb_dequeue_tail);
EXPORT_SYMBOL(cmb_queue_len);
EXPORT_SYMBOL(cmb_queue_purge);
EXPORT_SYMBOL(alloc_cmb);
EXPORT_SYMBOL(kfree_cmb);
EXPORT_SYMBOL(cmb_tailroom);
EXPORT_SYMBOL(cmb_reserve);
EXPORT_SYMBOL(cmb_put);
EXPORT_SYMBOL(cmb_push);
EXPORT_SYMBOL(cmb_pull);
EXPORT_SYMBOL(cmb_purge);
 
