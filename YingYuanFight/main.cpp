#include <iostream>
using namespace std;
#pragma warning(disable:4996)
#include <map>
#include <set>
#include <list>
#define RIVAL_NUM 4	// 对手数量

// 玩家数据
struct sPlayerData
{
	uint32_t	Rank;		// 排名
	uint64_t	PlayerID;	// 玩家id
};

// 选择对手的策略
enum eSelectStrategy
{
	eSelectStrategy_LeftRight = 1,	// 先左后右(左边是排名靠前的,优先找"强者")
	eSelectStrategy_RightLeft = 2,	// 先右后左(右边是排名靠后的,优先找"弱者")
};

class YingYuanFight
{
public:
	// 匹配算法(刷新也走这个流程)
	void Match(int64_t RoleID)
	{
		sPlayerData* pData = GetDataByRole(RoleID);
		if (!pData)
			return;

		uint32_t Rank = pData->Rank;
		// 排除掉的角色
		std::set<uint64_t> ExcludeRoleList;	//排除掉的角色列表
		ExcludeRoleList.insert(RoleID);

		// 找到的对手列表
		std::list<uint64_t> RivalRoleList;

		// 找不到人,则不继续查找,返回找到的部分数据.
		std::list<uint32_t> list_input;	// 4个区间变量
		if (list_input.size() != RIVAL_NUM)
			return;

		eSelectStrategy Strategy = eSelectStrategy_LeftRight;
		for (auto PercentValue : list_input)
		{
			// 加上随机 TODO
			uint32_t BeginPoint = (uint32_t)(Rank * (PercentValue % 100 / 100.f));
			uint64_t OutRoleID = 0;
			if (SelectOne(BeginPoint, Strategy, ExcludeRoleList, OutRoleID))
				RivalRoleList.push_back(OutRoleID); else
				break;
		}
		// 返回查找到的所有角色
	}

	// 选出一名对手
	bool SelectOne(uint32_t BeginPoint, eSelectStrategy Strategy, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		// 为false表示没有找到
		bool bFind = true;

		// 如果策略是先左后右
		if (Strategy == eSelectStrategy_LeftRight)
			if (!(bFind = FindLeft(BeginPoint, ExcludeRoleList, OutRoleID)))
				bFind = FindRight(BeginPoint, ExcludeRoleList, OutRoleID);
		else if (Strategy == eSelectStrategy_RightLeft)	// 先右后左
			if (!(bFind = FindRight(BeginPoint, ExcludeRoleList, OutRoleID)))
				bFind = FindLeft(BeginPoint, ExcludeRoleList, OutRoleID);
		return bFind;
	}

	// 向右查找
	bool FindRight(uint32_t BeginPoint, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		auto middle_iter = m_TotalRank.lower_bound(BeginPoint);
		for (auto iter = middle_iter; iter != m_TotalRank.end(); ++iter)
		{
			if (!iter->second)
				continue;
			if (ExcludeRoleList.find(iter->second->PlayerID) != ExcludeRoleList.end())
				continue;

			OutRoleID = iter->second->PlayerID;
			ExcludeRoleList.insert(OutRoleID);
			return true;
		}
		return false;
	}

	// 向左查找
	bool FindLeft(uint32_t BeginPoint, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		auto middle_iter = m_TotalRank.lower_bound(BeginPoint);
		RankList_Type::reverse_iterator reverse_iter(middle_iter);
		for (auto iter = reverse_iter; iter != m_TotalRank.rbegin(); ++iter)
		{
			if (!iter->second)
				continue;
			if (ExcludeRoleList.find(iter->second->PlayerID) != ExcludeRoleList.end())
				continue;

			OutRoleID = iter->second->PlayerID;
			ExcludeRoleList.insert(OutRoleID);
			return true;
		}
		return false;
	}

private:
	// 通过角色id获取排行数据
	sPlayerData* GetDataByRole(uint64_t RoleID)
	{
		if (m_RankList_Role.find(RoleID) != m_RankList_Role.end())
			return m_RankList_Role[RoleID];
		return nullptr;
	}

	// 根据排名获得排行数据
	sPlayerData* GetDataByRank(uint32_t Rank)
	{
		if (m_TotalRank.find(Rank) != m_TotalRank.end())
			return m_TotalRank[Rank];
		return NULL;
	}

private:
	typedef std::map<uint32_t, sPlayerData*> RankList_Type;			// 以排名为key
	typedef std::map<uint64_t, sPlayerData*> RankList_Role_Type;	// 以角色id为key
	
	RankList_Role_Type					m_RankList_Role;// 以角色id
	RankList_Type						m_TotalRank;	// 总榜
	std::map<uint8_t, RankList_Type>	m_CampRank;		// 阵营榜

};

int main()
{

	return 0;
}