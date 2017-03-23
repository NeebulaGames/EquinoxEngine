#pragma once
#include <MathGeoLib/include/Geometry/AABB.h>
#include "GameObject.h"

#define MAX_BUCKET_SIZE 4

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box) : box(box)
	{
		bucket.reserve(MAX_BUCKET_SIZE);
	}

	void Insert(GameObject* gameObject)
	{
		if (box.Intersects(gameObject->BoundingBox))
		{
			if (!childs.empty())
			{
				InsertInChilds(gameObject);
			}
			else if (bucket.size() == MAX_BUCKET_SIZE)
			{
				Partition();
				InsertInChilds(gameObject);
			}
			else
			{
				bucket.push_back(gameObject);
			}
		}
	}

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*> objects, const TYPE& primitive) const
	{
		if (primitive.Intersects(box))
		{
			for (std::vector<GameObject*>::const_iterator it = this->bucket.begin(); it != this->bucket.end(); ++it)
			{
				if (primitive.Intersects((*it)->BoundingBox))
					objects.push_back(*it);
			}

			for (int i = 0; i < 4; ++i)
				if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);
		}
	}

private:
	void Partition()
	{
		vec size = box.Size() / 2;

		float minX = box.MinX();
		float minY = box.MinY();
		float minZ = box.MinZ();

		float maxX = box.MaxX();
		float maxY = box.MaxY();
		float maxZ = box.MaxZ();

		// Top
		AABB box1(vec(minX, minY, minZ), vec(minX + size.x, minY + size.y, maxZ));
		childs.push_back(new QuadtreeNode(box1));
		AABB box2(vec(minX + size.x, minY, minZ), vec(maxX, minY + size.y, maxZ));
		childs.push_back(new QuadtreeNode(box2));

		// Bottom
		AABB box3(vec(minX, minY + size.y, minZ), vec(minX + size.x, maxY, maxZ));
		childs.push_back(new QuadtreeNode(box3));
		AABB box4(vec(minX + size.x, minY + size.y, minZ), vec(maxX, maxY, maxZ));
		childs.push_back(new QuadtreeNode(box4));

		for (GameObject* obj : bucket)
			InsertInChilds(obj);

		bucket.clear();
	}

	void InsertInChilds(GameObject* gameObject)
	{
		assert(!childs.empty());
		for (QuadtreeNode* child : childs)
			child->Insert(gameObject);
	}

	AABB box;
	std::vector<GameObject*> bucket;
	std::vector<QuadtreeNode*> childs;
};

class Quadtree
{
public:

	Quadtree(AABB& limits)
	{
		root = new QuadtreeNode(limits);
	}

	~Quadtree()
	{
	}

	void Clear()
	{
		Clear(root);
	}

	void Insert(GameObject* gameObject)
	{
		root->Insert(gameObject);
	}

	void Remove(GameObject* gameObject)
	{
		
	}

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*> objects, const TYPE& primitive) const
	{
		root->CollectIntersections(objects, primitive);
	}

private:
	void Clear(QuadtreeNode* node)
	{

	}

	QuadtreeNode* root;
};

