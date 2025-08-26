#pragma once
#include <vector>
#include <memory>
#include "Math.h"

class Tile;
enum class TileState;
class OpenSetMarker;
class CloseSetMarker;

class AStar
{
public:

	static std::vector<Tile*> PathFinding(const std::vector<std::vector<Tile*>>& tiles);

private:

	enum class NodeType
	{
		Default,
		Start,
		End,
		Wall
	};

	struct Node
	{
		Vector2 mNodeIdDebug{ 0,0 };
		std::vector<Node*> mAdjacent;
		Node* mParent = nullptr;
		Tile* mTile = nullptr;
		NodeType mNodeType = NodeType::Default;
		Vector2 mPos{ 0,0 };
		float f = 0.0f;
		float g = 0.0f;
		float h = 0.0f;
		bool bClose = false;
		bool bOpen = false;
		bool mBlocked = false;
	};

	static NodeType ConvertTileStateToNodeType(TileState state);
	static void BuildNode(const std::vector<std::vector<Tile*>>& tiles, std::vector<std::vector<Node>>& nodes, Node*& startNode, Node*& endNode);
	static float Heuristic(const Node& A, const Node& B);
	static void DrawDebug(const std::vector<std::vector<Node>>& nodes);

	static std::vector<std::weak_ptr<OpenSetMarker>> openSetMarkers;
	static std::vector<std::weak_ptr<CloseSetMarker>> closeSetMarkers;

};