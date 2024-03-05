t_treenode	*build_subtree_2(t_treenode *sub, t_treenode **lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	if (!(*lin_tree) || (*lin_tree)->bracket_lvl < bracket_lvl)
		return (sub);
	if ((*lin_tree)->bracket_lvl == bracket_lvl)
	{
		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		*lin_tree = (*lin_tree)->left;
		sub = build_subtree_2(sub, lin_tree, bracket_lvl);
	}
	else
	{
		new = build_subtree(NULL, lin_tree, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		if (!(*lin_tree))
			return (sub);
		sub = build_subtree_2(sub, lin_tree, (*lin_tree)->bracket_lvl);
	}
	return (sub);
}