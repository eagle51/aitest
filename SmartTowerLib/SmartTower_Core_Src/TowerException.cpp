//#include "StdAfx.h"
#include "TowerException.h"
#include <sstream>
namespace SmartTower
{
    //////////////////////////////////////////
    /////////////////////////////////////////
    ///该类已于2022年8月21日重构校队完成,李辉
    ///产权归陕西恒巨软件科技有限公司所有
    /////////////////////////////////////
    /////////////////////////////////////
    TowerException::TowerException(HandleObject ErrObject)
        :_ErrObject(ErrObject)
    {

    } 

    TowerException::TowerException(const char* ErrMsg) :
        _ErrMsg(ErrMsg), _ErrObject(0)
    {

    }

    TowerException::~TowerException(void)
    {
    }


    PointErrException::PointErrException(HandleTowerPoint ErrPoint, ErrType ErrInfo)
        :TowerException(ErrPoint.GetPointee()), _errInFo(ErrInfo)
    {
        ostringstream ErrMessage;
        CTowerPoint* point = static_cast<CTowerPoint*>(_ErrObject.GetPointee());
        ErrMessage << "节点编号：" << point->TagNum() << " ";
        switch (_errInFo)
        {
        case DuplicateNum:
            ErrMessage << "重复输入节点";
            break;
        case IsymErr:
            ErrMessage << "对称性错误,节点重复";
            break;
        case NumErr:
            ErrMessage << "节点编号不符合规则";
            break;
        case CNNPNotExsit:
            ErrMessage << "所利用的点无法找到";
            break;
        case RatioErr:
            ErrMessage << "一类节点的坐标值错误或二类节点的参数错误";
            break;
        case FoundationZErr:
            ErrMessage << "同一组长短腿中存在至少两个基础节点（非对称点）Z坐标相同";
            break;
        case FoundationIsymErr:
            ErrMessage << "基础节点的对称性不为4";
            break;
        case CoorErr:
            ErrMessage << "坐标值错误";
            break;
        case AuxABSCoorErr:
            ErrMessage << "程序不推荐使用绝对坐标点";
            break;
        }
        _ErrMsg = ErrMessage.str();
    }

    PointErrException::~PointErrException()
    {

    }

    MenberException::MenberException(HandleMemberInf ErrMenb, ErrType ErrInfo, int Tagnum)
        :TowerException(ErrMenb.GetPointee()), _errInFo(ErrInfo), _tagNum(Tagnum)
    {
        ostringstream ErrMessage;
        MemberInf *pMemb = static_cast<MemberInf*>(_ErrObject.GetPointee());
        ErrMessage << "杆件：" << pMemb->MinTagNum() << "--" << pMemb->MaxTagNum() << " ";
        switch (_errInFo)
        {
        case NodeNotFound:
        case OriginNodeNotFound:
            ErrMessage << "节点编号：" << _tagNum << "不存在，或者对称信息不正确";
            break;
        case ParaErr:
            ErrMessage << "输入参数错误";
            break;
        case Duplicate:
            ErrMessage << "重复输入杆件";
            break;
        case ZerosLength:
            ErrMessage << "杆件长度为0";
            break;
        case ErrPartInfo:
            ErrMessage << "杆件跨本体接腿表的分配范围";
            break;
        case SMCErr:
            ErrMessage << "杆件的SMC(同压)错误，或不存在交点";
            break;
        case L1L2Err:
            ErrMessage << "最小轴L1和平行轴L2填写错误";
            break;
        case TypeErr:
            ErrMessage << "类型数据错误";
            break;
        case ProfileWorning:
            ErrMessage << "材质不合理";
            break;
        case ErrDeductHols:
            ErrMessage << "减孔值错误";
            break;
        case AEfficWorning:
            ErrMessage << "应力比超限";
            break;
        case ASRWorning:
            ErrMessage << "长细比超限";
            break;
        case BoltEfficWorning:
            ErrMessage << "螺栓应力比超限";
            break;
        case MkindErr:
            ErrMessage << "约束信息错误，不符合当前规范约束信息填写";
            break;
        case MkindBSENErr:
            ErrMessage << "约束信息错误，请注意欧标规范单面连接一颗螺栓约束填法，以及在这种情况下不同标准之间的约束定义的转换";
            break;
        case ISymErr:
            ErrMessage << "杆件对称性错误";
            break;
        case ALenWeight:
            ErrMessage << "可能需要考虑人重长度";
            break;
        case RedundantCheckErr:
            ErrMessage << "按照辅助材平衡法校验应力比或者螺栓数量警告情况";
            break;
        case BlockCheckWorning:
            ErrMessage << "螺栓快剪超限，请核对";
            break;
        }
        _ErrMsg = ErrMessage.str();
    }

    MenberException::~MenberException()
    {

    }


    PartException::PartException(HandlePart ErrPart, int ErrTag, ErrType Errtp)
        :TowerException(ErrPart.GetPointee()), _ErrTag(ErrTag), _errtp(Errtp)
    {
        ostringstream ErrMessage;
        CBasePart *pPart = static_cast<CBasePart*>(_ErrObject.GetPointee());
        ErrMessage << "节点分配表，总行数：" << pPart->getPartIndex()+1 << " ";
        switch (_errtp)
        {
        case NodeNotFound:
            ErrMessage << "不存在或错误节点号" << _ErrTag;
            break;
        case ElmNotFound:
            ErrMessage << "与不存在节点相连的杆件：" << _ErrTag;
            break;
        case CNNErr:
            ErrMessage << "所要连接的本体不存在，或与该本体内不存在杆件！" << _ErrTag;
            break;
        case ERRNumUp:
            ErrMessage << "本段号码比下一段的大，或本段内的节点号大小不符合要求!" << _ErrTag;
            break;
        case FDTNumErr:
            ErrMessage << "长短腿基础节点之间的关系不明确！点号：" << _ErrTag;
            break;
        case ErrFmin:
            ErrMessage << "本体上与接腿部有直接连接的最小节点号Fmin错误，最小号码应该是：" << _ErrTag;
            break;
		case ExtendNumErr:
			ErrMessage << "延伸段信息错误，延伸段号码，应该大于公共号，小于最小基础点号：" << _ErrTag;
			break;
		case ExtendElmErr:
			ErrMessage << "延伸段信息错误，虽然定义了延伸段最小号，但是还没有延伸段杆件：" << _ErrTag;
			break;
        }
        _ErrMsg = ErrMessage.str();
    }

    PartException::~PartException()
    {

    }
}