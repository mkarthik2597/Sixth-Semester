void add_to_root_list(node* new_node)
{
  if(min_node == NULL)
  {
    new_node->left = new_node->right = new_node;
  }

  else
  {
    new_node->left = min_node;
    new_node->right = min_node->right;
    min_node->right = new_node;
    new_node->right->left = new_node;
  }
}

void add_to_child_list(node* new_node, node* head)
{
  if(head == NULL)
  {
    new_node->left = new_node->right = new_node;
    return;
  }

  new_node->left = head;
  new_node->right = head->right;
  head->right = new_node;
  new_node->right->left = new_node;
}

node* remove_from_child_list(node* head)
{
  head->left->right = head->right;
  head->right->left = head->left;

  node* next_child;

  if(head->right != head)
  next_child = head->right;
  else
  next_child = NULL;

  head->left = head->right = head;

  return next_child;
}

void remove_from_root_list(node* head)
{
  head->left->right = head->right;
  head->right->left = head->left;

  head->left = head->right = head;
}

void swap(node** x, node** y)
{
  node* temp = *x;
  *x = *y;
  *y = temp;
}
