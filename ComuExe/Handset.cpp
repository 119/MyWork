#include "ComuStdAfx.h"

static char TBL_CToG[] = 
{
      '1',                          /* ' ' */
      '2', '2', '2',                /* 'abc' */
	  '3', '3', '3',                /* 'def' */
      '4', '4', '4',                /* 'ghi' */
      '5', '5', '5',                /* 'jkl' */
      '6', '6', '6',                /* 'mno' */
      '7', '7', '7', '7',           /* 'pqrs' */
      '8', '8', '8',                /* 'tuv' */
      '9', '9', '9', '9'            /* 'wxyz' */
};

static  const char PY_mb_space0[]={"����������������������������"};
static  const char PY_mb_space1[]={"����棥����"};
static  const char PY_mb_space2[]={"�¡����������֡١ܡݣ����ڡ�"};
static  const char PY_mb_a[]     ={"������߹�����"};
static  const char PY_mb_ai[]    ={"������������������������������������"};
static  const char PY_mb_an[]    ={"�����������������������������"};
static  const char PY_mb_ang[]   ={"������"};
static  const char PY_mb_ao[]    ={"�°����������İð��������������"};
static  const char PY_mb_ba[]    ={"�Ѱ˰ɰְͰΰհǰӰ̰ϰŰ԰Ȱаư��ΰ���"};
static  const char PY_mb_bai[]   ={"�װٰڰݰܰذ۲������"};
static  const char PY_mb_ban[]   ={"����������߰��������"};
static  const char PY_mb_bang[]  ={"�������������������"};
static  const char PY_mb_bao[]   ={"�����������������������ұ����ٱ����������������"};
static  const char PY_mb_bei[]   ={"�������������������±�������������㣱�"};
static  const char PY_mb_ben[]   ={"���������������"};
static  const char PY_mb_beng[]  ={"���ı±ű��ð����"};
static  const char PY_mb_bi[]    ={"�ȱرʱձǱϱܱƱ˱̱ұ۱ڱɱֱӱ�ذ�ױ�赱�����澱������"};
static  const char PY_mb_bian[]  ={"�߱��������ޱ�������������"};
static  const char PY_mb_biao[]  ={"����������������"};
static  const char PY_mb_bie[]   ={"�������"};
static  const char PY_mb_bin[]   ={"�����������ޱ�����������"};
static  const char PY_mb_bing[]  ={"����������������������������"};
static  const char PY_mb_bo[]    ={"�����������������������������˲����ز���������������뢲���"};
static  const char PY_mb_bu[]    ={"��������������������������߲�"};
static  const char PY_mb_ca[]    ={"������"};
static  const char PY_mb_cai[]   ={"�Ų̲˲ƲɲĲòʲ²Ȳ�"};
static  const char PY_mb_can[]   ={"�βв��ҲѲͲϲ������"};
static  const char PY_mb_cang[]  ={"�ֲԲײز���"};
static  const char PY_mb_cao[]   ={"�ݲܲٲ۲�����"};
static  const char PY_mb_ce[]    ={"��߲�����"};
static  const char PY_mb_cen[]   ={"����"};
static  const char PY_mb_ceng[]  ={"��������"};
static  const char PY_mb_cha[]   ={"���������ɲ����������"};
static  const char PY_mb_chai[]  ={"�����٭�����"};
static  const char PY_mb_chan[]  ={"�������������������������Ƶ������"};
static  const char PY_mb_chang[] ={"���������������������ѳ����곫���ϲ�"};
static  const char PY_mb_chao[]  ={"�������������������˽˴�"};
static  const char PY_mb_che[]   ={"�����������߳���"};
static  const char PY_mb_chen[]  ={"�³Ƴó��������������ӳ����׳�"};
static  const char PY_mb_cheng[] ={"�ɳ˳�ʢ�ųƳǳʳϳӳͳгȳѳ���ة�����"};
static  const char PY_mb_chi[]   ={"�Գٳس߳�ճ�ݳֳܳ�ڳ۳޳�߳���׳��������"};
static  const char PY_mb_chong[] ={"���س������������"};
static  const char PY_mb_chou[]  ={"�����������������"};
static  const char PY_mb_chu[]   ={"������������������������������������ۻ������ء"};
static  const char PY_mb_chuai[] ={"���������"};
static  const char PY_mb_chuan[] ={"�������������������"};
static  const char PY_mb_chuang[]={"��������������"};
static  const char PY_mb_chui[]  ={"����������׵���"};
static  const char PY_mb_chun[]  ={"�������������ȴ�ݻ"};
static  const char PY_mb_chuo[]  ={"����������"};
static  const char PY_mb_ci[]    ={"�δ˴ʴɴǴ̲��Ŵȴƴ��ȴĴô�������"};
static  const char PY_mb_cong[]  ={"�ӴԴдҴϴ�������"};
static  const char PY_mb_cou[]   ={"������"};
static  const char PY_mb_cu[]    ={"�ִ����⧴�����������"};
static  const char PY_mb_cuan[]  ={"�ܴڴ�����ߥ��"};
static  const char PY_mb_cui[]   ={"�ߴ�޴�ݴ��ʹ�����"};
static  const char PY_mb_cun[]   ={"����ߗ����"};
static  const char PY_mb_cuo[]   ={"�������ȴ�����"};
static  const char PY_mb_da[]    ={"�������������������"};
static  const char PY_mb_dai[]   ={"������������������������ᷴ���"};
static  const char PY_mb_dan[]   ={"��������������������������ʯ���鵬������"};
static  const char PY_mb_dang[]  ={"��������������������"};
static  const char PY_mb_dao[]   ={"����������������������߶������"};
static  const char PY_mb_de[]    ={"�ĵصõµ��"};
static  const char PY_mb_dei[]   ={"��"};
static  const char PY_mb_deng[]  ={"�ȵƵ˵ǳεɵʵ����"};
static  const char PY_mb_di[]    ={"�صڵ׵͵ݵĵܵеҵֵ��ֵӵ���Ե̵յѵ����ڮ�Ƶ޵�ۡ"};
static  const char PY_mb_dia[]   ={"��"};
static  const char PY_mb_dian[]  ={"��������ߵ������������ڵ����"};
static  const char PY_mb_diao[]  ={"������������������"};
static  const char PY_mb_die[]   ={"������������������"};
static  const char PY_mb_ding[]  ={"���������������������������"};
static  const char PY_mb_diu[]   ={"����"};
static  const char PY_mb_dong[]  ={"����������������������ἶ�"};
static  const char PY_mb_dou[]   ={"����������񼶶�������"};
static  const char PY_mb_du[]    ={"���ȶ¶Ŷ��ɶ��Ƕ����ƶĶö�����빶��"};
static  const char PY_mb_duan[]  ={"�ζ̶϶˶Ͷ����"};
static  const char PY_mb_dui[]   ={"�ԶӶѶ���"};
static  const char PY_mb_dun[]   ={"�ٶֶ׶ܶڶն���۶�������"};
static  const char PY_mb_duo[]   ={"�����������޶�������"};
static  const char PY_mb_e[]     ={"��Ŷ���󰢶��������㵶�ج����������"};
static  const char PY_mb_en[]    ={"��������"};
static  const char PY_mb_er[]    ={"��������������������"};
static  const char PY_mb_fa[]    ={"������������������"};
static  const char PY_mb_fan[]   ={"����������������������������������"};
static  const char PY_mb_fang[]  ={"�ŷ������ķ��÷·���������"};
static  const char PY_mb_fei[]   ={"�ǷɷʷѷηϷ˷зͷ������������������������"};
static  const char PY_mb_fen[]   ={"�ַݷҷ۷ܷطշڷ߷׷޷����ٷ�"};
static  const char PY_mb_feng[]  ={"����������������ٺ��"};
static  const char PY_mb_fo[]    ={"��"};
static  const char PY_mb_fou[]   ={"���"};
static  const char PY_mb_fu[]    ={"���򸸷��������������������������������𸥷��������������������������������������������ܽ����������𥸬������ݳ��"};
static  const char PY_mb_ga[]    ={"�¸���٤��길���"};
static  const char PY_mb_gai[]   ={"�ĸøǸŸ�ؤ��������"};
static  const char PY_mb_gan[]   ={"�ɸϸʸиҸ͸θ̸˸���ߦ���������"};
static  const char PY_mb_gang[]  ={"�ոָ۸ٸڸܸԸظ���"};
static  const char PY_mb_gao[]   ={"�߸����۬�ݸ����غ�����ھ"};
static  const char PY_mb_ge[]    ={"����������������쿩�������ت��������"};
static  const char PY_mb_gei[]   ={"��"};
static  const char PY_mb_gen[]   ={"����ب����ݢ"};
static  const char PY_mb_geng[]  ={"�����������������ٹ�"};
static  const char PY_mb_gong[]  ={"����������������������������������"};
static  const char PY_mb_gou[]   ={"��������������������ڸ�����������"};
static  const char PY_mb_gu[]    ={"�ŹɹĹȹʹ˹¹ù̹͹����ǹ�����������Ƽ���������������"};
static  const char PY_mb_gua[]   ={"�ҹιϹ��Թй����ڴ"};
static  const char PY_mb_guai[]  ={"�ֹԹ���"};
static  const char PY_mb_guan[]  ={"�عܹٹ۹ݹ߹޹�ڹ�ݸ������������"};
static  const char PY_mb_guang[] ={"����������"};
static  const char PY_mb_gui[]   ={"����������������������������"};
static  const char PY_mb_gun[]   ={"������������"};
static  const char PY_mb_guo[]   ={"����������������������"};
static  const char PY_mb_ha[]    ={"����Ϻ��"};
static  const char PY_mb_hai[]   ={"�������Ⱥ������˺���"};
static  const char PY_mb_han[]   ={"������������������������嫺���������������ۺ����ʺ���"};
static  const char PY_mb_hang[]  ={"���к����캻��"};
static  const char PY_mb_hao[]   ={"�úźƺ����º����ƺ���婸�����"};
static  const char PY_mb_he[]    ={"�ͺϺκȺӺ˺ɺغǺкֺ̺׺ҺպԺ��������"};
static  const char PY_mb_hei[]   ={"�ں�"};
static  const char PY_mb_hen[]   ={"�ܺݺ޺�"};
static  const char PY_mb_heng[]  ={"����ߺ�������"};
static  const char PY_mb_hong[]  ={"���������������ڧ��"};
static  const char PY_mb_hou[]   ={"����������"};
static  const char PY_mb_hu[]    ={"��������������������������������������������������"};
static  const char PY_mb_hua[]   ={"�����������������������"};
static  const char PY_mb_huai[]  ={"������������"};
static  const char PY_mb_huan[]  ={"���������������û»�����ۼ���������"};
static  const char PY_mb_huang[] ={"�ƻŻʻλĻѻ˻ɻ̻ͻȻǻл����������"};
static  const char PY_mb_hui[]   ={"�ػ�һ�ӻԻ�ٻֻջڻܻ���߻޻�ݻۻ�����ڶ�����"};
static  const char PY_mb_hun[]   ={"�������������ڻ"};
static  const char PY_mb_huo[]   ={"����������������"};
static  const char PY_mb_ji[]    ={"�����������Ǽƻ����ȼ��̼��ļ����ͼʻ���������ϵ���������������������������˼ɼ��û�������ꪼ��߼��弧��������᧼�����٥������ؽ"};
static  const char PY_mb_jia[]   ={"�ҼӼټۼּ׼ܼмѼμԼռݼ޼�Ю��٤��ᵼ�����"};
static  const char PY_mb_jian[]  ={"����������ὥ�����콣������������������߼������������������������"};
static  const char PY_mb_jiang[] ={"�����������������������������������"};
static  const char PY_mb_jiao[]  ={"���нŽǽ̽����Ͻ�У�νƽ��ʽȽ������þ��������ӽ�𨽸�˽ɽѽͽ�"};
static  const char PY_mb_jie[]   ={"�ӽڽ�����ֽٽܽ��ҽ��ؽԽݽ޽߽�׽��ڦ����������ݽ�"};
static  const char PY_mb_jin[]   ={"���������������������������������������������"};
static  const char PY_mb_jing[]  ={"����������������������������������������������ݼ����溾���"};
static  const char PY_mb_jiong[] ={"������"};
static  const char PY_mb_jiu[]   ={"�;žƾȾ��þɾ˾¾ʾ̾ľ��ξǾ������������"};
static  const char PY_mb_ju[]    ={"��־پ޾�߾Ӿ�ݽ۾վؾܾ����۾�ԾھѾ׾о�������������"};
static  const char PY_mb_juan[]  ={"��Ȧ���������������"};
static  const char PY_mb_jue[]   ={"�������Ǿ�������������Ȿ��ާ��"};
static  const char PY_mb_jun[]   ={"�����������������������޹���"};
static  const char PY_mb_ka[]    ={"�������ǿ�"};
static  const char PY_mb_kai[]   ={"��������������������"};
static  const char PY_mb_kan[]   ={"��������٩�������"};
static  const char PY_mb_kang[]  ={"����������������"};
static  const char PY_mb_kao[]   ={"������������"};
static  const char PY_mb_ke[]    ={"�ɿ˿ƿ̿ͿÿſοǿȿʿĿ��¿����������"};
static  const char PY_mb_ken[]   ={"�Ͽпѿ�"};
static  const char PY_mb_keng[]  ={"�ӿ��"};
static  const char PY_mb_kong[]  ={"�տؿ׿���"};
static  const char PY_mb_kou[]   ={"�ڿۿ�ߵ��ޢ��"};
static  const char PY_mb_ku[]    ={"��޿�ݿ������"};
static  const char PY_mb_kua[]   ={"������"};
static  const char PY_mb_kuai[]  ={"����������"};
static  const char PY_mb_kuan[]  ={"�����"};
static  const char PY_mb_kuang[] ={"�������������ڲڿ"};
static  const char PY_mb_kui[]   ={"����������������������ظ������"};
static  const char PY_mb_kun[]   ={"�������������"};
static  const char PY_mb_kuo[]   ={"����������"};
static  const char PY_mb_la[]    ={"���������������������"};
static  const char PY_mb_lai[]   ={"���������������"};
static  const char PY_mb_lan[]   ={"���������������������������������"};
static  const char PY_mb_lang[]  ={"��������ݹ��������"};
static  const char PY_mb_lao[]   ={"��������������������������"};
static  const char PY_mb_le[]    ={"��������߷"};
static  const char PY_mb_lei[]   ={"������������������������������"};
static  const char PY_mb_leng[]  ={"������㶠"};
static  const char PY_mb_li[]    ={"����������������������������������������������������������������������ݰ�ٳ�������������"};
static  const char PY_mb_lia[]   ={"��"};
static  const char PY_mb_lian[]  ={"����������������������������������"};
static  const char PY_mb_liang[] ={"����������������������������"};
static  const char PY_mb_liao[]  ={"����������������������������"};
static  const char PY_mb_lie[]   ={"��������������"};
static  const char PY_mb_lin[]   ={"������������������������������"};
static  const char PY_mb_ling[]  ={"�����������������������������������"};
static  const char PY_mb_liu[]   ={"���������������������������"};
static  const char PY_mb_long[]  ={"��£����¡����¢��¤��"};
static  const char PY_mb_lou[]   ={"¥§©¶¦¨ª�������"};
static  const char PY_mb_lu[]    ={"·¶½¼¬¯¹±»³®²­«¸¾°º�´��µ���"};
static  const char PY_mb_luan[]  ={"�����������������"};
static  const char PY_mb_lue[]   ={"����"};
static  const char PY_mb_lun[]   ={"����������������"};
static  const char PY_mb_luo[]   ={"����������������������������"};
static  const char PY_mb_lv[]    ={"������������������¿�������������"};
static  const char PY_mb_ma[]    ={"������������Ĩ�������"};
static  const char PY_mb_mai[]   ={"��������������۽"};
static  const char PY_mb_man[]   ={"������������������á���"};
static  const char PY_mb_mang[]  ={"æâäãçå����"};
static  const char PY_mb_mao[]   ={"ëðéèñòìîïó���êí����"};
static  const char PY_mb_me[]    ={"ô"};
static  const char PY_mb_mei[]   ={"ûÿ��ü÷öý��úõ��������ݮøùþ��"};
static  const char PY_mb_men[]   ={"�����������"};
static  const char PY_mb_meng[]  ={"���������������������������"};
static  const char PY_mb_mi[]    ={"�����������������������������������"};
static  const char PY_mb_mian[]  ={"����������������������"};
static  const char PY_mb_miao[]  ={"���������������������"};
static  const char PY_mb_mie[]   ={"��������ؿ"};
static  const char PY_mb_min[]   ={"�������������������"};
static  const char PY_mb_ming[]  ={"������������ڤ�������"};
static  const char PY_mb_miu[]   ={"����"};
static  const char PY_mb_mo[]    ={"��ĩĥĪĨİīĬħûġģĮĤ����ĦĢĭ��į������"};
static  const char PY_mb_mou[]   ={"ĳıĲ��"};
static  const char PY_mb_mu[]    ={"ľĸĶĿ����ļĹĻ��ģĽĺĵķĴ����"};
static  const char PY_mb_na[]    ={"������������������"};
static  const char PY_mb_nai[]   ={"������������ؾ"};
static  const char PY_mb_nan[]   ={"��������������"};
static  const char PY_mb_nang[]  ={"����"};
static  const char PY_mb_nao[]   ={"����������ث���"};
static  const char PY_mb_ne[]    ={"��������ګ"};
static  const char PY_mb_nei[]   ={"��������"};
static  const char PY_mb_nen[]   ={"���"};
static  const char PY_mb_neng[]  ={"��"};
static  const char PY_mb_ni[]    ={"�������������������������������"};
static  const char PY_mb_nian[]  ={"����ճ�����������շإ��"};
static  const char PY_mb_niang[] ={"����"};
static  const char PY_mb_niao[]  ={"������"};
static  const char PY_mb_nie[]   ={"�������������������"};
static  const char PY_mb_nin[]   ={"���"};
static  const char PY_mb_ning[]  ={"����š����Ţ������"};
static  const char PY_mb_niu[]   ={"ţťŤŦ�������"};
static  const char PY_mb_nong[]  ={"ũŪŨŧٯ��"};
static  const char PY_mb_nu[]    ={"Ŭŭū����"};
static  const char PY_mb_nuan[]  ={"ů"};
static  const char PY_mb_nue[]   ={"Űű��"};
static  const char PY_mb_nuo[]   ={"ŲŵųŴ����"};
static  const char PY_mb_nv[]    ={"Ů"};
static  const char PY_mb_o[]     ={"Ŷ���"};
static  const char PY_mb_ou[]    ={"ŷżźŹŸ��ŻŽ��ک"};
static  const char PY_mb_pa[]    ={"����ſž����������"};
static  const char PY_mb_pai[]   ={"������������"};
static  const char PY_mb_pan[]   ={"�����������������"};
static  const char PY_mb_pang[]  ={"�������Ӱ�������հ�"};
static  const char PY_mb_pao[]   ={"������������������"};
static  const char PY_mb_pei[]   ={"������������������"};
static  const char PY_mb_pen[]   ={"����"};
static  const char PY_mb_peng[]  ={"������������������������������"};
static  const char PY_mb_pi[]    ={"Ƥ����ƥ����Ƣơ��ƣƦƨ��اƧƩ���������"};
static  const char PY_mb_pian[]  ={"Ƭƪƭƫ�����"};
static  const char PY_mb_piao[]  ={"ƱƮƯư���α������"};
static  const char PY_mb_pie[]   ={"ƲƳ"};
static  const char PY_mb_pin[]   ={"ƷƶƸƴƵ����"};
static  const char PY_mb_ping[]  ={"ƽƾƿ����ƹƼƻƺ�"};
static  const char PY_mb_po[]    ={"���������������ǲ���۶����"};
static  const char PY_mb_pou[]   ={"����"};
static  const char PY_mb_pu[]    ={"�������������������Ը�������������"};
static  const char PY_mb_qi[]    ={"�����������������������������������������������������������������������������������ޭݽ�"};
static  const char PY_mb_qia[]   ={"ǡǢ����"};
static  const char PY_mb_qian[]  ={"ǰǮǧǣǳǷǱǨǦǫǩǶǸǥǯǪǤǬǭǲǴǵ��ٻ������"};
static  const char PY_mb_qiang[] ={"ǿǹ��ǽǻǺǾǼ��������"};
static  const char PY_mb_qiao[]  ={"������������������������������ڽ��"};
static  const char PY_mb_qie[]   ={"��������������"};
static  const char PY_mb_qin[]   ={"������������������������������"};
static  const char PY_mb_qing[]  ={"����������������������������"};
static  const char PY_mb_qiong[] ={"�����������"};
static  const char PY_mb_qiu[]   ={"�������������������ó��������"};
static  const char PY_mb_qu[]    ={"ȥ��ȡ����Ȣ��������Ȥ��������ȣ���"};
static  const char PY_mb_quan[]  ={"ȫȨȰȦȭȮȪȬȯ��ڹ�ȩȧ"};
static  const char PY_mb_que[]   ={"ȴȱȷȸȵȳȶ��Ȳ"};
static  const char PY_mb_qun[]   ={"Ⱥȹ����"};
static  const char PY_mb_ran[]   ={"ȻȾȼȽ����"};
static  const char PY_mb_rang[]  ={"����ȿ�����"};
static  const char PY_mb_rao[]   ={"���������"};
static  const char PY_mb_re[]    ={"����"};
static  const char PY_mb_ren[]   ={"�����������������������"};
static  const char PY_mb_reng[]  ={"����"};
static  const char PY_mb_ri[]    ={"��"};
static  const char PY_mb_rong[]  ={"������������������������"};
static  const char PY_mb_rou[]   ={"��������"};
static  const char PY_mb_ru[]    ={"���������������������"};
static  const char PY_mb_ruan[]  ={"������"};
static  const char PY_mb_rui[]   ={"�����������"};
static  const char PY_mb_run[]   ={"����"};
static  const char PY_mb_ruo[]   ={"����ټ"};
static  const char PY_mb_sa[]    ={"����ئ�����"};
static  const char PY_mb_sai[]   ={"����������"};
static  const char PY_mb_san[]   ={"��ɢɡ��"};
static  const char PY_mb_sang[]  ={"ɣɥɤ��"};
static  const char PY_mb_sao[]   ={"ɨɩɦɧ��"};
static  const char PY_mb_se[]    ={"ɫɬɪ����"};
static  const char PY_mb_sen[]   ={"ɭ"};
static  const char PY_mb_seng[]  ={"ɮ"};
static  const char PY_mb_sha[]   ={"ɱɳɴɵɶɷ��ɲɰɯ������"};
static  const char PY_mb_shai[]  ={"ɹɸ��ɫ"};
static  const char PY_mb_shan[]  ={"ɽ��������ɼ����������ɾɺ���ɿڨɻ������"};
static  const char PY_mb_shang[] ={"��������������������"};
static  const char PY_mb_shao[]  ={"����������������������"};
static  const char PY_mb_she[]   ={"������������������������������"};
static  const char PY_mb_shei[]  ={"˭"};
static  const char PY_mb_shen[]  ={"��ʲ������������������������������ݷ����"};
static  const char PY_mb_sheng[] ={"����ʡʢʤʣʥ������������"};
static  const char PY_mb_shi[]   ={"����ʱʮʶʵʧ��ʯʦʾʽ��ʷʹʼ����������ʫʩʨʪʿʰʳ��������ʬ��ʻ�ų���ʺʸ����ʴ����ʭ����������"};
static  const char PY_mb_shou[]  ={"�����������������������"};
static  const char PY_mb_shu[]   ={"��������������������ˡ�����������������������������������������������"};
static  const char PY_mb_shua[]  ={"ˢˣ�"};
static  const char PY_mb_shuai[] ={"ˤ˦˧��˥�"};
static  const char PY_mb_shuan[] ={"˩˨����"};
static  const char PY_mb_shuang[]={"˫˪ˬ��"};
static  const char PY_mb_shui[]  ={"ˮ˭˰˯˵"};
static  const char PY_mb_shun[]  ={"˳˱˲˴"};
static  const char PY_mb_shuo[]  ={"˵˶˸˷��"};
static  const char PY_mb_si[]    ={"��˾˿��˽˼˹˻˺�����������������������"};
static  const char PY_mb_song[]  ={"����������������ݿ��ڡ��"};
static  const char PY_mb_sou[]   ={"��������������"};
static  const char PY_mb_su[]    ={"�����������������������������"};
static  const char PY_mb_suan[]  ={"������"};
static  const char PY_mb_sui[]   ={"��������������������������"};
static  const char PY_mb_sun[]   ={"������ݥ������"};
static  const char PY_mb_suo[]   ={"���������������������"};
static  const char PY_mb_ta[]    ={"������̤����̣�̡̢�������"};
static  const char PY_mb_tai[]   ={"̧̨̫̬̩̥̦̭̪��"};
static  const char PY_mb_tan[]   ={"̸̷̲̾̽��̵̶̴̰̹̳̯̱̼̻̺̮̿�����"};
static  const char PY_mb_tang[]  ={"�����������������������������"};
static  const char PY_mb_tao[]   ={"���������������������������"};
static  const char PY_mb_te[]    ={"��߯��"};
static  const char PY_mb_teng[]  ={"��������"};
static  const char PY_mb_ti[]    ={"����������������������������������"};
static  const char PY_mb_tian[]  ={"������������������"};
static  const char PY_mb_tiao[]  ={"���������������٬����"};
static  const char PY_mb_tie[]   ={"������"};
static  const char PY_mb_ting[]  ={"��ͣͦ��ͤͥͧ͢͡��������"};
static  const char PY_mb_tong[]  ={"ͬͨʹͳͭͯͰ١ͱͮͲͩͪͫ����"};
static  const char PY_mb_tou[]   ={"ͷͶ͵͸��"};
static  const char PY_mb_tu[]    ={"��ͻͼͿ����;ͽ͹ͺ��ݱ��"};
static  const char PY_mb_tuan[]  ={"�����"};
static  const char PY_mb_tui[]   ={"������������߯"};
static  const char PY_mb_tun[]   ={"���Ͷ��������"};
static  const char PY_mb_tuo[]   ={"������������������٢������������"};
static  const char PY_mb_wa[]    ={"�������������ݰ��"};
static  const char PY_mb_wai[]   ={"������"};
static  const char PY_mb_wan[]   ={"����������������������������������ݸ��"};
static  const char PY_mb_wang[]  ={"�����������������������"};
static  const char PY_mb_wei[]   ={"ΪλδΧί��ΣζΤΰβθι��΢οκΡΥΨΩάαγέήηξνε�Φ��μ��Ϋޱ��"};
static  const char PY_mb_wen[]   ={"����������������������������"};
static  const char PY_mb_weng[]  ={"��������޳"};
static  const char PY_mb_wo[]    ={"���������������������ݫ��"};
static  const char PY_mb_wu[]    ={"����������������������������������������������������������������������"};
static  const char PY_mb_xi[]    ={"��ϴϸϢϣϧϰ��ϲϵϯϡϦϷ��ϥ����Ϫ����ϤϮ϶������������ϱϨ�����ϫϬϭϩ������"};
static  const char PY_mb_xia[]   ={"������ϿϺϹϼ����ϻ��ϾϽ���������"};
static  const char PY_mb_xian[]  ={"��������������������������������������������������ϳ�������޺"};
static  const char PY_mb_xiang[] ={"������������������������������������������"};
static  const char PY_mb_xiao[]  ={"СЧЦ��У��Т��Ф����������������Х��������"};
static  const char PY_mb_xie[]   ={"дЩлЭЯЪбЬйкжгавЫмеизШ�������"};
static  const char PY_mb_xin[]   ={"����������оп��н�����ܰ"};
static  const char PY_mb_xing[]  ={"��������������������������ʡ����ߩ"};
static  const char PY_mb_xiong[] ={"��������������"};
static  const char PY_mb_xiu[]   ={"����������������������"};
static  const char PY_mb_xu[]    ={"������������������������������������������ޣ������"};
static  const char PY_mb_xuan[]  ={"ѡ����������ѢѤѣ�����������"};
static  const char PY_mb_xue[]   ={"ѧѩѪ��ѦѨѥ"};
static  const char PY_mb_xun[]   ={"ѰѸѶѲѯѮѫѵѱѳ�ѴѭѷѬ��"};
static  const char PY_mb_ya[]    ={"ѽѹ��Ѻ��Ѿ��Ѽ��ѿ��ѻ���������"};
static  const char PY_mb_yan[]   ={"����������������������������������������������������������������������������ٲ��������������������"};
static  const char PY_mb_yang[]  ={"�����������������������������������������"};
static  const char PY_mb_yao[]   ={"Ҫҡҧҩ������ҫҢҨԿҦҤҥң��ôߺ���Ƚ������"};
static  const char PY_mb_ye[]    ={"ҲҹҵҳҰүҶҺҮұҷҬҭҴҸ��"};
static  const char PY_mb_yi[]    ={"һ������������������ҽ��������������������������Ҿ������������������������������������������Ҽ��߮��������������ҿܲ����"};
static  const char PY_mb_yin[]   ={"����ӡ���������������������������ط���"};
static  const char PY_mb_ying[]  ={"ӦӲӰӳӪӢӭӱӤӣӨӫөӬӮӥ��ӯ��ӧ���"};
static  const char PY_mb_yo[]    ={"Ӵ�"};
static  const char PY_mb_yong[]  ={"������ӵӿӹӾӽӶӺӷ���ӼӻӸܭ��"};
static  const char PY_mb_you[]   ={"��������������������������������������������ݬ��"};
static  const char PY_mb_yu[]    ={"��������������������Ԥ����������ԥ����ԣԡ�������������������������Ԣ��خ����������ع��������Ԧ����������"};
static  const char PY_mb_yuan[]  ={"ԶԱԪԺ԰ԲԹԭԸԴԵԩԬԮԨԳԯԷԧԫ"};
static  const char PY_mb_yue[]   ={"��ԽԼԾ��������Ի��"};
static  const char PY_mb_yun[]   ={"����������������������ܿ��������۩���"};
static  const char PY_mb_za[]    ={"����զ������"};
static  const char PY_mb_zai[]   ={"������������������"};
static  const char PY_mb_zan[]   ={"�������������"};
static  const char PY_mb_zang[]  ={"�����߲����"};
static  const char PY_mb_zao[]   ={"����������������������������"};
static  const char PY_mb_ze[]    ={"�����������"};
static  const char PY_mb_zei[]   ={"��"};
static  const char PY_mb_zen[]   ={"����"};
static  const char PY_mb_zeng[]  ={"������������"};
static  const char PY_mb_zha[]   ={"��ը��բգեէթ������ա����"};
static  const char PY_mb_zhai[]  ={"ժխծ��լիկ"};
static  const char PY_mb_zhan[]  ={"վռչսմճձջղնհտ��յո��շպ"};
static  const char PY_mb_zhang[] ={"���³�����������������������������"};
static  const char PY_mb_zhao[]  ={"�������������ֳ���������צ��"};
static  const char PY_mb_zhe[]   ={"��������������������������������"};
static  const char PY_mb_zhen[]  ={"����������������������������������֡����������"};
static  const char PY_mb_zheng[] ={"��������֣����֤����������֢���"};
static  const char PY_mb_zhi[]   ={"ֱֻ֪֮��ָֹ֧������־ֵ������ַ��ְִֶֽ֦֥֭֯��ֲֳּ֬����ֿ��ֺ֫֩������֨������ۤ��"};
static  const char PY_mb_zhong[] ={"����������������������ڣ"};
static  const char PY_mb_zhou[]  ={"�������������������������������"};
static  const char PY_mb_zhu[]   ={"ףס������ע������������������פ������������������������"};
static  const char PY_mb_zhua[]  ={"ץצ"};
static  const char PY_mb_zhuai[] ={"ק"};
static  const char PY_mb_zhuan[] ={"תר׬ש��׫׭�"};
static  const char PY_mb_zhuang[]={"װײׯ׳״׮ױ��"};
static  const char PY_mb_zhui[]  ={"׷׹׺׵׶׸���"};
static  const char PY_mb_zhun[]  ={"׼׻���"};
static  const char PY_mb_zhuo[]  ={"׽����׾����׿�������������"};
static  const char PY_mb_zi[]    ={"��������������������������������"};
static  const char PY_mb_zong[]  ={"����������������"};
static  const char PY_mb_zou[]   ={"��������"};
static  const char PY_mb_zu[]    ={"������������������"};
static  const char PY_mb_zuan[]  ={"����׬߬��"};
static  const char PY_mb_zui[]   ={"���������"};
static  const char PY_mb_zun[]   ={"����ߤ����"};
static  const char PY_mb_zuo[]   ={"��������������������������"};


/*
********************************************************************************
*                  DEFINE PINYIN CODE TABLE INDEX
********************************************************************************
*/
static  const PY_INDEX PY_index_all[] = {{" ",PY_mb_space0},
                                        {" ",PY_mb_space1},
                                        {" ",PY_mb_space2},
                                        {"a",PY_mb_a},
                                        {"ai",PY_mb_ai},
                                        {"an",PY_mb_an},
                                        {"ang",PY_mb_ang},
                                        {"ao",PY_mb_ao},
                                        {"ba",PY_mb_ba},
                                        {"bai",PY_mb_bai},
                                        {"ban",PY_mb_ban},
                                        {"bang",PY_mb_bang},
                                        {"bao",PY_mb_bao},
                                        {"bei",PY_mb_bei},
                                        {"ben",PY_mb_ben},
                                        {"beng",PY_mb_beng},
                                        {"bi",PY_mb_bi},
                                        {"bian",PY_mb_bian},
                                        {"biao",PY_mb_biao},
                                        {"bie",PY_mb_bie},
                                        {"bin",PY_mb_bin},
                                        {"bing",PY_mb_bing},
                                        {"bo",PY_mb_bo},
                                        {"bu",PY_mb_bu},
                                        {"ca",PY_mb_ca},
                                        {"cai",PY_mb_cai},
                                        {"can",PY_mb_can},
                                        {"cang",PY_mb_cang},
                                        {"cao",PY_mb_cao},
                                        {"ce",PY_mb_ce},
                                        {"cen",PY_mb_cen},
                                        {"ceng",PY_mb_ceng},
                                        {"cha",PY_mb_cha},
                                        {"chai",PY_mb_chai},
                                        {"chan",PY_mb_chan},
                                        {"chang",PY_mb_chang},
                                        {"chao",PY_mb_chao},
                                        {"che",PY_mb_che},
                                        {"chen",PY_mb_chen},
                                        {"cheng",PY_mb_cheng},
                                        {"chi",PY_mb_chi},
                                        {"chong",PY_mb_chong},
                                        {"chou",PY_mb_chou},
                                        {"chu",PY_mb_chu},
                                        {"chuai",PY_mb_chuai},
                                        {"chuan",PY_mb_chuan},
                                        {"chuang",PY_mb_chuang},
                                        {"chui",PY_mb_chui},
                                        {"chun",PY_mb_chun},
                                        {"chuo",PY_mb_chuo},
                                        {"ci",PY_mb_ci},
                                        {"cong",PY_mb_cong},
                                        {"cou",PY_mb_cou},
                                        {"cu",PY_mb_cu},
                                        {"cuan",PY_mb_cuan},
                                        {"cui",PY_mb_cui},
                                        {"cun",PY_mb_cun},
                                        {"cuo",PY_mb_cuo},
                                        {"da",PY_mb_da},
                                        {"dai",PY_mb_dai},
                                        {"dan",PY_mb_dan},
                                        {"dang",PY_mb_dang},
                                        {"dao",PY_mb_dao},
                                        {"de",PY_mb_de},
                                        {"dei",PY_mb_dei},
                                        {"deng",PY_mb_deng},
                                        {"di",PY_mb_di},
                                        {"dia",PY_mb_dia},
                                        {"dian",PY_mb_dian},
                                        {"diao",PY_mb_diao},
                                        {"die",PY_mb_die},
                                        {"ding",PY_mb_ding},
                                        {"diu",PY_mb_diu},
                                        {"dong",PY_mb_dong},
                                        {"dou",PY_mb_dou},
                                        {"du",PY_mb_du},
                                        {"duan",PY_mb_duan},
                                        {"dui",PY_mb_dui},
                                        {"dun",PY_mb_dun},
                                        {"duo",PY_mb_duo},
                                        {"e",PY_mb_e},
                                        {"en",PY_mb_en},
                                        {"er",PY_mb_er},
                                        {"fa",PY_mb_fa},
                                        {"fan",PY_mb_fan},
                                        {"fang",PY_mb_fang},
                                        {"fei",PY_mb_fei},
                                        {"fen",PY_mb_fen},
                                        {"feng",PY_mb_feng},
                                        {"fo",PY_mb_fo},
                                        {"fou",PY_mb_fou},
                                        {"fu",PY_mb_fu},
                                        {"ga",PY_mb_ga},
                                        {"gai",PY_mb_gai},
                                        {"gan",PY_mb_gan},
                                        {"gang",PY_mb_gang},
                                        {"gao",PY_mb_gao},
                                        {"ge",PY_mb_ge},
                                        {"gei",PY_mb_gei},
                                        {"gen",PY_mb_gen},
                                        {"geng",PY_mb_geng},
                                        {"gong",PY_mb_gong},
                                        {"gou",PY_mb_gou},
                                        {"gu",PY_mb_gu},
                                        {"gua",PY_mb_gua},
                                        {"guai",PY_mb_guai},
                                        {"guan",PY_mb_guan},
                                        {"guang",PY_mb_guang},
                                        {"gui",PY_mb_gui},
                                        {"gun",PY_mb_gun},
                                        {"guo",PY_mb_guo},
                                        {"ha",PY_mb_ha},
                                        {"hai",PY_mb_hai},
                                        {"han",PY_mb_han},
                                        {"hang",PY_mb_hang},
                                        {"hao",PY_mb_hao},
                                        {"he",PY_mb_he},
                                        {"hei",PY_mb_hei},
                                        {"hen",PY_mb_hen},
                                        {"heng",PY_mb_heng},
                                        {"hong",PY_mb_hong},
                                        {"hou",PY_mb_hou},
                                        {"hu",PY_mb_hu},
                                        {"hua",PY_mb_hua},
                                        {"huai",PY_mb_huai},
                                        {"huan",PY_mb_huan},
                                        {"huang",PY_mb_huang},
                                        {"hui",PY_mb_hui},
                                        {"hun",PY_mb_hun},
                                        {"huo",PY_mb_huo},
                                        {"ji",PY_mb_ji},
                                        {"jia",PY_mb_jia},
                                        {"jian",PY_mb_jian},
                                        {"jiang",PY_mb_jiang},
                                        {"jiao",PY_mb_jiao},
                                        {"jie",PY_mb_jie},
                                        {"jin",PY_mb_jin},
                                        {"jing",PY_mb_jing},
                                        {"jiong",PY_mb_jiong},
                                        {"jiu",PY_mb_jiu},
                                        {"ju",PY_mb_ju},
                                        {"juan",PY_mb_juan},
                                        {"jue",PY_mb_jue},
                                        {"jun",PY_mb_jun},
                                        {"ka",PY_mb_ka},
                                        {"kai",PY_mb_kai},
                                        {"kan",PY_mb_kan},
                                        {"kang",PY_mb_kang},
                                        {"kao",PY_mb_kao},
                                        {"ke",PY_mb_ke},
                                        {"ken",PY_mb_ken},
                                        {"keng",PY_mb_keng},
                                        {"kong",PY_mb_kong},
                                        {"kou",PY_mb_kou},
                                        {"ku",PY_mb_ku},
                                        {"kua",PY_mb_kua},
                                        {"kuai",PY_mb_kuai},
                                        {"kuan",PY_mb_kuan},
                                        {"kuang",PY_mb_kuang},
                                        {"kui",PY_mb_kui},
                                        {"kun",PY_mb_kun},
                                        {"kuo",PY_mb_kuo},
                                        {"la",PY_mb_la},
                                        {"lai",PY_mb_lai},
                                        {"lan",PY_mb_lan},
                                        {"lang",PY_mb_lang},
                                        {"lao",PY_mb_lao},
                                        {"le",PY_mb_le},
                                        {"lei",PY_mb_lei},
                                        {"leng",PY_mb_leng},
                                        {"li",PY_mb_li},
                                        {"lia",PY_mb_lia},
                                        {"lian",PY_mb_lian},
                                        {"liang",PY_mb_liang},
                                        {"liao",PY_mb_liao},
                                        {"lie",PY_mb_lie},
                                        {"lin",PY_mb_lin},
                                        {"ling",PY_mb_ling},
                                        {"liu",PY_mb_liu},
                                        {"long",PY_mb_long},
                                        {"lou",PY_mb_lou},
                                        {"lu",PY_mb_lu},
                                        {"luan",PY_mb_luan},
                                        {"lue",PY_mb_lue},
                                        {"lun",PY_mb_lun},
                                        {"luo",PY_mb_luo},
                                        {"lv",PY_mb_lv},
                                        {"ma",PY_mb_ma},
                                        {"mai",PY_mb_mai},
                                        {"man",PY_mb_man},
                                        {"mang",PY_mb_mang},
                                        {"mao",PY_mb_mao},
                                        {"me",PY_mb_me},
                                        {"mei",PY_mb_mei},
                                        {"men",PY_mb_men},
                                        {"meng",PY_mb_meng},
                                        {"mi",PY_mb_mi},
                                        {"mian",PY_mb_mian},
                                        {"miao",PY_mb_miao},
                                        {"mie",PY_mb_mie},
                                        {"min",PY_mb_min},
                                        {"ming",PY_mb_ming},
                                        {"miu",PY_mb_miu},
                                        {"mo",PY_mb_mo},
                                        {"mou",PY_mb_mou},
                                        {"mu",PY_mb_mu},
                                        {"na",PY_mb_na},
                                        {"nai",PY_mb_nai},
                                        {"nan",PY_mb_nan},
                                        {"nang",PY_mb_nang},
                                        {"nao",PY_mb_nao},
                                        {"ne",PY_mb_ne},
                                        {"nei",PY_mb_nei},
                                        {"nen",PY_mb_nen},
                                        {"neng",PY_mb_neng},
                                        {"ni",PY_mb_ni},
                                        {"nian",PY_mb_nian},
                                        {"niang",PY_mb_niang},
                                        {"niao",PY_mb_niao},
                                        {"nie",PY_mb_nie},
                                        {"nin",PY_mb_nin},
                                        {"ning",PY_mb_ning},
                                        {"niu",PY_mb_niu},
                                        {"nong",PY_mb_nong},
                                        {"nu",PY_mb_nu},
                                        {"nuan",PY_mb_nuan},
                                        {"nue",PY_mb_nue},
                                        {"nuo",PY_mb_nuo},
                                        {"nv",PY_mb_nv},
                                        {"o",PY_mb_o},
                                        {"ou",PY_mb_ou},
                                        {"pa",PY_mb_pa},
                                        {"pai",PY_mb_pai},
                                        {"pan",PY_mb_pan},
                                        {"pang",PY_mb_pang},
                                        {"pao",PY_mb_pao},
                                        {"pei",PY_mb_pei},
                                        {"pen",PY_mb_pen},
                                        {"peng",PY_mb_peng},
                                        {"pi",PY_mb_pi},
                                        {"pian",PY_mb_pian},
                                        {"piao",PY_mb_piao},
                                        {"pie",PY_mb_pie},
                                        {"pin",PY_mb_pin},
                                        {"ping",PY_mb_ping},
                                        {"po",PY_mb_po},
                                        {"pou",PY_mb_pou},
                                        {"pu",PY_mb_pu},
                                        {"qi",PY_mb_qi},
                                        {"qia",PY_mb_qia},
                                        {"qian",PY_mb_qian},
                                        {"qiang",PY_mb_qiang},
                                        {"qiao",PY_mb_qiao},
                                        {"qie",PY_mb_qie},
                                        {"qin",PY_mb_qin},
                                        {"qing",PY_mb_qing},
                                        {"qiong",PY_mb_qiong},
                                        {"qiu",PY_mb_qiu},
                                        {"qu",PY_mb_qu},
                                        {"quan",PY_mb_quan},
                                        {"que",PY_mb_que},
                                        {"qun",PY_mb_qun},
                                        {"ran",PY_mb_ran},
                                        {"rang",PY_mb_rang},
                                        {"rao",PY_mb_rao},
                                        {"re",PY_mb_re},
                                        {"ren",PY_mb_ren},
                                        {"reng",PY_mb_reng},
                                        {"ri",PY_mb_ri},
                                        {"rong",PY_mb_rong},
                                        {"rou",PY_mb_rou},
                                        {"ru",PY_mb_ru},
                                        {"ruan",PY_mb_ruan},
                                        {"rui",PY_mb_rui},
                                        {"run",PY_mb_run},
                                        {"ruo",PY_mb_ruo},
                                        {"sa",PY_mb_sa},
                                        {"sai",PY_mb_sai},
                                        {"san",PY_mb_san},
                                        {"sang",PY_mb_sang},
                                        {"sao",PY_mb_sao},
                                        {"se",PY_mb_se},
                                        {"sen",PY_mb_sen},
                                        {"seng",PY_mb_seng},
                                        {"sha",PY_mb_sha},
                                        {"shai",PY_mb_shai},
                                        {"shan",PY_mb_shan},
                                        {"shang",PY_mb_shang},
                                        {"shao",PY_mb_shao},
                                        {"she",PY_mb_she},
                                        {"shei",PY_mb_shei},
                                        {"shen",PY_mb_shen},
                                        {"sheng",PY_mb_sheng},
                                        {"shi",PY_mb_shi},
                                        {"shou",PY_mb_shou},
                                        {"shu",PY_mb_shu},
                                        {"shua",PY_mb_shua},
                                        {"shuai",PY_mb_shuai},
                                        {"shuan",PY_mb_shuan},
                                        {"shuang",PY_mb_shuang},
                                        {"shui",PY_mb_shui},
                                        {"shun",PY_mb_shun},
                                        {"shuo",PY_mb_shuo},
                                        {"si",PY_mb_si},
                                        {"song",PY_mb_song},
                                        {"sou",PY_mb_sou},
                                        {"su",PY_mb_su},
                                        {"suan",PY_mb_suan},
                                        {"sui",PY_mb_sui},
                                        {"sun",PY_mb_sun},
                                        {"suo",PY_mb_suo},
                                        {"ta",PY_mb_ta},
                                        {"tai",PY_mb_tai},
                                        {"tan",PY_mb_tan},
                                        {"tang",PY_mb_tang},
                                        {"tao",PY_mb_tao},
                                        {"te",PY_mb_te},
                                        {"teng",PY_mb_teng},
                                        {"ti",PY_mb_ti},
                                        {"tian",PY_mb_tian},
                                        {"tiao",PY_mb_tiao},
                                        {"tie",PY_mb_tie},
                                        {"ting",PY_mb_ting},
                                        {"tong",PY_mb_tong},
                                        {"tou",PY_mb_tou},
                                        {"tu",PY_mb_tu},
                                        {"tuan",PY_mb_tuan},
                                        {"tui",PY_mb_tui},
                                        {"tun",PY_mb_tun},
                                        {"tuo",PY_mb_tuo},
                                        {"wa",PY_mb_wa},
                                        {"wai",PY_mb_wai},
                                        {"wan",PY_mb_wan},
                                        {"wang",PY_mb_wang},
                                        {"wei",PY_mb_wei},
                                        {"wen",PY_mb_wen},
                                        {"weng",PY_mb_weng},
                                        {"wo",PY_mb_wo},
                                        {"wu",PY_mb_wu},
                                        {"xi",PY_mb_xi},
                                        {"xia",PY_mb_xia},
                                        {"xian",PY_mb_xian},
                                        {"xiang",PY_mb_xiang},
                                        {"xiao",PY_mb_xiao},
                                        {"xie",PY_mb_xie},
                                        {"xin",PY_mb_xin},
                                        {"xing",PY_mb_xing},
                                        {"xiong",PY_mb_xiong},
                                        {"xiu",PY_mb_xiu},
                                        {"xu",PY_mb_xu},
                                        {"xuan",PY_mb_xuan},
                                        {"xue",PY_mb_xue},
                                        {"xun",PY_mb_xun},
                                        {"ya",PY_mb_ya},
                                        {"yan",PY_mb_yan},
                                        {"yang",PY_mb_yang},
                                        {"yao",PY_mb_yao},
                                        {"ye",PY_mb_ye},
                                        {"yi",PY_mb_yi},
                                        {"yin",PY_mb_yin},
                                        {"ying",PY_mb_ying},
                                        {"yo",PY_mb_yo},
                                        {"yong",PY_mb_yong},
                                        {"you",PY_mb_you},
                                        {"yu",PY_mb_yu},
                                        {"yuan",PY_mb_yuan},
                                        {"yue",PY_mb_yue},
                                        {"yun",PY_mb_yun},
                                        {"za",PY_mb_za},
                                        {"zai",PY_mb_zai},
                                        {"zan",PY_mb_zan},
                                        {"zang",PY_mb_zang},
                                        {"zao",PY_mb_zao},
                                        {"ze",PY_mb_ze},
                                        {"zei",PY_mb_zei},
                                        {"zen",PY_mb_zen},
                                        {"zeng",PY_mb_zeng},
                                        {"zha",PY_mb_zha},
                                        {"zhai",PY_mb_zhai},
                                        {"zhan",PY_mb_zhan},
                                        {"zhang",PY_mb_zhang},
                                        {"zhao",PY_mb_zhao},
                                        {"zhe",PY_mb_zhe},
                                        {"zhen",PY_mb_zhen},
                                        {"zheng",PY_mb_zheng},
                                        {"zhi",PY_mb_zhi},
                                        {"zhong",PY_mb_zhong},
                                        {"zhou",PY_mb_zhou},
                                        {"zhu",PY_mb_zhu},
                                        {"zhua",PY_mb_zhua},
                                        {"zhuai",PY_mb_zhuai},
                                        {"zhuan",PY_mb_zhuan},
                                        {"zhuang",PY_mb_zhuang},
                                        {"zhui",PY_mb_zhui},
                                        {"zhun",PY_mb_zhun},
                                        {"zhuo",PY_mb_zhuo},
                                        {"zi",PY_mb_zi},
                                        {"zong",PY_mb_zong},
                                        {"zou",PY_mb_zou},
                                        {"zu",PY_mb_zu},
                                        {"zuan",PY_mb_zuan},
                                        {"zui",PY_mb_zui},
                                        {"zun",PY_mb_zun},
                                        {"zuo",PY_mb_zuo}
                                       };

/***************HstCtl**********************************************************/
static char CMD_PowerOn[]			= {"PE"};	
static char CMD_PowerOff[]			= {"PD"};
static char CMD_LightOn[]			= {"LL"};
static char CMD_LightOff[]			= {"EL"};
static char CMD_RingingType[]		= {"AR"};
static char CMD_RingingReq[]		= {"RR"};
static char CMD_RingingCancle[]		= {"CR"};
static char CMD_KeysTone[]			= {"KR"};
static char CMD_AdContrast[]		= {"SV"};
static char CMD_AuxShow[]			= {"AC"};
static char CMD_ChkHandle[]			= {"RE"};

static char CMD_InitLcd[]			= {"RS"};
static char CMD_PointerPos[]		= {"MV"};
static char CMD_StrDisplay[]		= {"DH"};
static char CMD_AntiScreen[]		= {"SR"};
static char CMD_CursorCtl[]			= {"CC"};

/***************KeyBoard**********************************************************/
static const INT8U KEY_0[]       = {2, '0', ' '};                                           /* '0' */
static const INT8U KEY_1[]       = {9, '1', '.', ',', '#', '*', '+', '\\', '[', ']'};       /* '1' */
static const INT8U KEY_2[]       = {4, '2', 'A', 'B', 'C'};                                 /* '2' */
static const INT8U KEY_3[]       = {4, '3', 'D', 'E', 'F'};                                 /* '3' */
static const INT8U KEY_4[]       = {4, '4', 'G', 'H', 'I'};                                 /* '4' */
static const INT8U KEY_5[]       = {4, '5', 'J', 'K', 'L'};                                 /* '5' */
static const INT8U KEY_6[]       = {4, '6', 'M', 'N', 'O'};                                 /* '6' */
static const INT8U KEY_7[]       = {5, '7', 'P', 'Q', 'R', 'S'};                            /* '7' */
static const INT8U KEY_8[]       = {4, '8', 'T', 'U', 'V'};                                 /* '8' */
static const INT8U KEY_9[]       = {5, '9', 'W', 'X', 'Y', 'Z'};                            /* '9' */
static const INT8U KEY_STAR[]    = {2, '+', '*'};                                           /* 'a'<-->'*' */
static const INT8U KEY_JING[]    = {1, '#'};                                                /* 'b'<-->'#' */

static const INT8U *CONVERTKEY[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, 
										 KEY_6, KEY_7, KEY_8, KEY_9, KEY_STAR, KEY_JING};
static const INT8U FCNKEYCOLLECT[]  = {     KEY_ACK, KEY_NAK, KEY_SND, KEY_UP, KEY_DOWN,
                                            PTT_ON, PTT_OFF, PTT_HOLD, KEY_NOOPERATE, KEY_END, 
											KEY_PWR_ON, KEY_SHORTCUT, KEY_NULL, KEY_NULL, KEY_PWR_OFF, 
											KEY_FCN,KEY_LOOSEN, KEY_FCN, KEY_MEDICAL, KEY_TROUBLE,
											KEY_INFORM, KEY_VOICE };

static char KB_CHINESE[] = {"������������������������"};
static char KB_ASCII[]  = {".,?!:;#$%^'\"/&=()-+%@_<>"};
static INT8U PT_BLANK[]	= {"�޼�¼"};

//��ʼ����̬����
INT16U	CHandset::LinePos[MAX_LINE];
INT16U	CHandset::CurLine;
INT16U	CHandset::PreLine;
INT16U	CHandset::MaxLine;
INT8S	CHandset::curstack;
INT8U	CHandset::winattrib;
bool	CHandset::numlocked;
bool	CHandset::uppercase;			//???
INT8U	CHandset::recvkey;
INT8U	CHandset::prekey;
INT8U	CHandset::hitnum;
INT8U	CHandset::m_textptr[300];
INT16U	CHandset::linepos[MAXLINE];
INT16U	CHandset::maxline;
INT16U	CHandset::curline;
INT16U	CHandset::textlen;

//HANDLE	CHandset::m_port;

ITEMMENU_STRUCT	CHandset::MENU_CHINESE;
ITEMMENU_STRUCT	CHandset::MENU_UPPER;
ITEMMENU_STRUCT	CHandset::MENU_LOWER;
ITEMMENU_STRUCT CHandset::MENU_DIGITAL;
ITEMMENU_STRUCT CHandset::MENU_SELINPUT_CHILD[4];
ITEMMENU_STRUCT	CHandset::MENU_SELINPUT;
ITEMMENU_STRUCT	CHandset::ItemMenu_cur;
EDITOR_STRUCT	CHandset::Editor;
CHandset::EDITORBUF_STRUCT	CHandset::EditorBuf;
CHandset::STACKITEM_STRUCT	CHandset::winstack[MAX_STACK];
CHandset::ExtKB	CHandset::m_extkb;
CHandset::RECORD_STRUCT CHandset::record;
CHandset::FMENUWIN_STRUCT CHandset::fmenuwin;
//StackLst	CHandset::key_stack;
KEY_STRUCT	CHandset::m_curkey_kb;
CHandset::LMENUWIN_STRUCT	CHandset::lmenuwin;
CHandset::RECORD_STRUCT		CHandset::record_LMenu[MAX_RECORD];
MENUWIN_STRUCT	CHandset::MenuWin_cur;
CHandset::WINDOWENTRY_STRUCT	CHandset::WinEntry;
PROMPT_STRUCT	CHandset::Prompt;
PROMPT_STRUCT	CHandset::PromptEntry;
CHandset::WINDOWENTRY_STRUCT	CHandset::WinEntry_Prompt;
CHandset::SCREEN_STRUCT	CHandset::screen;
CHandset::TEXTWIN_STRUCT	CHandset::textwin;
StackLst	CHandset::keymsg_stack;

WINSTACKENTRY_STRUCT CHandset::WinStackEntry_Prompt;
WINSTACKENTRY_STRUCT CHandset::WinStackEntry_Editor;
WINSTACKENTRY_STRUCT CHandset::WinStackEntry_SelChn;

KEY_STRUCT CHandset::m_curkey_editor;
KEY_STRUCT CHandset::m_curkey_extkb;
KEY_STRUCT CHandset::m_curkey_menuwin;
KEY_STRUCT CHandset::m_curkey_prompt;
KEY_STRUCT CHandset::m_curkey_py;

//CHandset::DataFrame CHandset::HDFrame;
//CRITICAL_SECTION CHandset::m_cs_handset;

//hxd 090518
CHandset::keyword_struct CHandset::keyword;// keyword_struct keyword
CHandset::initchar_struct CHandset::initchar;
CHandset::SCB_STRUCT CHandset::SCB;

ulong	g_hsttm = 0;
	
DWORD g_dwPromptTime = 0;  //cyh add:add  ��ʾ������ʱ�䣬��λ:s
DWORD g_dwTmPromptStart = 0;  //cyh add:add  ��ʾ��ʼ��ʱ��

void *G_ThreadHandAnalyse(void *arg)
{
	g_hst.P_ThreadHandAnalyse();
}

void G_TmHstConver(void *arg, int len)
{
	g_hst.P_TmHstConvert();
}

#undef MSG_HEAD
#define MSG_HEAD ("Comu-Handset")

//////////////////////////////////////////////////////////////////////////

CHandset::CHandset()
{
	memset(LinePos, 0, MAX_LINE);
	CurLine = 0;
	PreLine = 0;
	MaxLine = 0;
	curstack = -1;
	winattrib = 0;
	numlocked = true;
	uppercase = true;
	recvkey = 0;
	prekey = KEY_NULL;
	hitnum = 0;
	memset(m_textptr, 0, 300);
	memset(linepos, 0, MAXLINE);
	maxline = 0;
	curline = 0;
	textlen = 0;
//	m_port = NULL;

	keymsg_stack.reset();
	memset(&m_curkey_menuwin, 0, sizeof(m_curkey_menuwin));
	memset(&m_curkey_prompt, 0, sizeof(m_curkey_prompt));
	memset(&m_curkey_extkb, 0, sizeof(m_curkey_extkb));
	memset(&m_curkey_editor, 0, sizeof(m_curkey_editor));
	memset(&m_curkey_kb, 0, sizeof(m_curkey_kb));
	memset(&m_curkey_py, 0, sizeof(m_curkey_py));

	m_pthreadHandlWork = 0;

	InitMeun();
}

CHandset::~CHandset()
{

}

int CHandset::Init()
{	
	if( pthread_create(&m_pthreadHandlWork, NULL, G_ThreadHandAnalyse, NULL) != 0 )
	{
		PRTMSG(MSG_ERR, "create handle work failed!\n");
		perror("");
		return ERR_THREAD;
	}

	return 0;
}

int CHandset::Release()
{

}

void CHandset::P_ThreadHandAnalyse()
{
	byte  bytLvl = 0;
	char  szRecvBuf[1024] = {0};
	DWORD dwRecvLen = 0;
	DWORD dwPushTm;
	DWORD dwPackNum;
	DWORD dwBufSize = (DWORD)sizeof(szRecvBuf);

	while( !g_bProgExit )
	{
		usleep(50000);

		dwRecvLen = 0;
		memset(szRecvBuf, 0, sizeof(szRecvBuf));

		// �������ֱ��õ�������
		if( !m_objHandleReadMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			if( dwRecvLen <= dwBufSize )
			{
				if (szRecvBuf[0]==0x6d)		//'m'
				{
					TurnonHst();	
					char buf[2] = {0x0c,0x01};
					g_objDiaoduOrHandWorkMng.PushData(LV1, 2, buf, dwPackNum);
				}
				
				ParseKeyBoard(szRecvBuf[0]);
			}
		}
	}
}

void CHandset::InitMeun()
{
	MENU_CHINESE.Attrib = ITEM_FCN;
	MENU_CHINESE.ChildItem = 0;
	memcpy(MENU_CHINESE.ContentStr, "����", 4);
	MENU_CHINESE.EntryProc = Sel_Chinese;
	MENU_CHINESE.NumChild = 0;

	MENU_UPPER.Attrib = ITEM_FCN;
	MENU_UPPER.ChildItem = 0;
	memcpy(MENU_UPPER.ContentStr, "��д", 4);
	MENU_UPPER.EntryProc = Sel_Upper;
	MENU_UPPER.NumChild = 0;

	MENU_LOWER.Attrib = ITEM_FCN;
	MENU_LOWER.ChildItem = 0;
	memcpy(MENU_LOWER.ContentStr, "Сд", 4);
	MENU_LOWER.EntryProc = Sel_Lower;
	MENU_LOWER.NumChild = 0;

	MENU_DIGITAL.Attrib = ITEM_FCN;
	MENU_DIGITAL.ChildItem = 0;
	memcpy(MENU_DIGITAL.ContentStr, "����", 4);
	MENU_DIGITAL.EntryProc = Sel_Digital;
	MENU_DIGITAL.NumChild = 0;

	MENU_SELINPUT.Attrib = ITEM_LMENU;
	MENU_SELINPUT.ChildItem = (ITEMMENU_STRUCT *)MENU_SELINPUT_CHILD;
	memset(MENU_SELINPUT.ContentStr, 0, 20);
	MENU_SELINPUT.EntryProc = 0;
	MENU_SELINPUT.NumChild = 0;

	memset(&EditorBuf, 0, sizeof(EditorBuf));

	WinStackEntry_Prompt.DestroyProc = DestroyProc_Prompt;
	WinStackEntry_Prompt.HdlKeyProc = HdlKeyProc_Prompt;
	WinStackEntry_Prompt.InitProc = InitProc_Prompt;
	WinStackEntry_Prompt.ResumeProc = ResumeProc_Prompt;
	WinStackEntry_Prompt.SuspendProc = SuspendProc_Prompt;

	WinStackEntry_Editor.DestroyProc = DestroyProc_Editor;
	WinStackEntry_Editor.HdlKeyProc = HdlKeyProc_Editor;
	WinStackEntry_Editor.InitProc = InitProc_Editor;
	WinStackEntry_Editor.ResumeProc = ResumeProc_Editor;
	WinStackEntry_Editor.SuspendProc = SuspendProc_Editor;

	WinStackEntry_SelChn.DestroyProc = DestroyProc_SelChn;
	WinStackEntry_SelChn.HdlKeyProc = HdlKeyProc_SelChn;
	WinStackEntry_SelChn.InitProc = InitProc_SelChn;
	WinStackEntry_SelChn.ResumeProc = ResumeProc_SelChn;
	WinStackEntry_SelChn.SuspendProc = SuspendProc_SelChn;
	
}

ulong CHandset::comm_write(char *data, int len)
{
	ulong ulLen = (ulong)g_objComAdjust.ComWrite(data, len);

	return ulLen;
}

/*
*********************************************************************
*						�����ֱ�����֡
*	cmd:	����ʶ����
*	data:	��������
*	len:	���ݳ���
*********************************************************************
*/
bool CHandset::SendHstFrame(char *cmd, char *data, int len)
{
	int i = 0;
	char buf[1500];

	memset(buf, 0, 1500);

#if USE_COMBUS == 1
	buf[i++] = 0xff;	// ʹ�ô�����չ��ʱ������������������Ҫ��QianExe�����CComBusת��
	buf[i++] = 0xf3;	// 0xf3��ʾ����չ�е�������3���з���
#endif

#if USE_COMBUS == 2
	buf[i++] = 0xff;	// ʹ��150TRʱ���������������ֱ�������Ҫ��QianExe�����CCom150TRת��
	buf[i++] = 0xA0;	// 0xA0��ʾ��150TR����չ���ڽ��з���
	buf[i++] = 0x01;	// ��չ����1
#endif

	buf[ i++ ] = (char)0xFF;
	memcpy(buf+i, cmd, 2);
	i += 2;
	memcpy(buf+i, data, len);

	i += len;
	buf[ i++ ] = (char)0xFF;
	
	ulong ret = comm_write(buf, i);
	
	return ret ? true : false;
}

bool CHandset::GetKeyMsg(KEY_STRUCT *key)
{
	INT8U buf[2];
	ulong l = keymsg_stack.pop((INT8U *)buf);
	
	if (l > 0)
	{
		key->attrib = buf[0];
		key->key = buf[1];

		return true;
	} 
	else
	{
		return false;
	}
}

/********************Editor****************************************************/
void CHandset::SelInputMode(void)	//����ģʽ
{
    switch(EditorBuf.Input)
    {
        case INPUT_CHINESE:
            LockNum();
            break;
        case INPUT_UPPER:
            UnlockNum();
            ToggleUpperCase();
            break;
        case INPUT_LOWER:
            UnlockNum();
            ToggleLowerCase();
            break;
        case INPUT_DIGITAL:
            LockNum();
            break;
        default:
            LockNum();
            break;
    }
}

void CHandset::Sel_Chinese(void)	//����
{
    PopMenuWin();
    EditorBuf.Input = INPUT_CHINESE;
    SelInputMode();
}

void CHandset::Sel_Upper(void)	//��д
{
    PopMenuWin();
    EditorBuf.Input = INPUT_UPPER;
    SelInputMode();
}

void CHandset::Sel_Lower(void)	//Сд
{
    PopMenuWin();
    EditorBuf.Input = INPUT_LOWER;
    SelInputMode();
}

void CHandset::Sel_Digital(void)	//����
{
    PopMenuWin();
    EditorBuf.Input = INPUT_DIGITAL;
    SelInputMode();
}

INT16U CHandset::InitRecordProc(void)
{
	static void (*Sample[])(void) = {Sel_Chinese, Sel_Upper, Sel_Lower, Sel_Digital};
    INT8U i;
    
    for (i = 0; i < MENU_SELINPUT.NumChild; i++)
	{
        if (MENU_SELINPUT_CHILD[i].EntryProc == Sample[EditorBuf.Input]) 
			return i;
    }
    
    return 0;
}

bool CHandset::ActivateSelInputMode(void)
{
    if (Editor.Mode & EDT_DIGITAL) 	
	{   
		return FALSE;   
	}
	else if (Editor.Mode & EDT_CHINESE)
	{
        MENU_SELINPUT_CHILD[0] = MENU_CHINESE;
        MENU_SELINPUT_CHILD[1] = MENU_UPPER;
        MENU_SELINPUT_CHILD[2] = MENU_LOWER;
        MENU_SELINPUT_CHILD[3] = MENU_DIGITAL;
        MENU_SELINPUT.NumChild = 4;
        ActivateItemMenu_Init(&MENU_SELINPUT, InitRecordProc);
        return TRUE;
    }
	else 
	{
        if (Editor.Mode & EDT_LOCKUPPER)
		{
            MENU_SELINPUT_CHILD[0] = MENU_UPPER;
            MENU_SELINPUT_CHILD[1] = MENU_DIGITAL;
            MENU_SELINPUT.NumChild = 2;
        }
		else 
		{
            MENU_SELINPUT_CHILD[0] = MENU_UPPER;
            MENU_SELINPUT_CHILD[1] = MENU_LOWER;
            MENU_SELINPUT_CHILD[2] = MENU_DIGITAL;
            MENU_SELINPUT.NumChild = 3;
        }

        ActivateItemMenu_Init(&MENU_SELINPUT, InitRecordProc);
		
        return TRUE;
    }
}

INT16U CHandset::GetMaxLen(void)
{
    if (Editor.MaxLenProc != 0) 
	{
        return Editor.MaxLenProc(EditorBuf.CurLen - Editor.LabelLen, &EditorBuf.Buffer[Editor.LabelLen]);
    } 
	else
	{
        return Editor.MaxLen;
    }
}

INT16U CHandset::GetMinLen(void)
{
    if (Editor.MinLenProc != 0) 
	{
        return Editor.MinLenProc(EditorBuf.CurLen - Editor.LabelLen, &EditorBuf.Buffer[Editor.LabelLen]);
    }
	else
	{
        return Editor.MinLen;
    }
}

INT16U CHandset::GetCurLen(void)
{
    if (Editor.CurLenProc != 0) 
	{
        return (Editor.LabelLen + 
        Editor.CurLenProc(EditorBuf.CurLen - Editor.LabelLen, &EditorBuf.Buffer[Editor.LabelLen]));
    } 
	else
	{
        return EditorBuf.CurLen;
    }
}

void CHandset::CheckEditorValid(void)
{
    INT16U maxlen, minlen, curlen;
    
    maxlen = GetMaxLen();
    minlen = GetMinLen();
    curlen = GetCurLen();
    if (Editor.LabelLen >= GetScreenSize()) return;
    if (minlen > maxlen) return;
    if (maxlen + Editor.LabelLen > sizeof(EditorBuf.Buffer)) return;
    if (curlen < Editor.LabelLen) return;
    if (curlen > Editor.LabelLen + maxlen) return;
}

//cyh add:  �������ڱ༭�����ݵ���ʾ����
void CHandset::SetLinePara(void)
{
    INT16U i, linesize;
    
    linesize = GetScreenLineSize();    
    MaxLine  = PlotLinePos(EditorBuf.Buffer, EditorBuf.CurLen, LinePos, MAX_LINE, linesize);
   
	if (MaxLine == 0xff) return;

	//cyh add: �ڱ༭ʱ��������ݳ�����һ���ˣ����������У�����¼���е���ʼλ��
    if (MaxLine > 0 && LinePos[MaxLine - 1] + linesize == EditorBuf.CurLen) 
	{
        if (MaxLine + 1 > MAX_LINE) return;
        LinePos[MaxLine] = EditorBuf.CurLen;
        MaxLine++;
    }
    
    PreLine = CurLine;
    if (MaxLine == 0) 
        CurLine = 0;
    else 
        CurLine = MaxLine - 1;

    for (i = 0; i < MaxLine; i++) 
	{
        if (EditorBuf.EditPos < LinePos[i]) 
		{
            CurLine = i - 1;
            break;
        }
    }
}

INT16U CHandset::GetInitCursorPos(void)
{
    INT16U linesize, numline, temp;
    
    if (MaxLine == 0) return 0;
    
    linesize = GetScreenLineSize();
    numline  = GetScreenNumLine();
    
	//�������һ��,>24���ַ�
	if (CurLine > numline - 1)
	{	
        temp = numline - 1;
    } 
	else 
	{
        temp = CurLine;
    }
    return temp * linesize + (EditorBuf.EditPos - LinePos[CurLine]);
}

void CHandset::UpdateWin_Editor(bool CanShowCursor)
{
    INT16U ScreenSize, LeftLen, StartPos, EndPos, i, j;
    INT16U lineno, startline;    

    ScreenSize = GetScreenSize();
    VTL_ClearScreen();                /* Clear Screen */

    if (Editor.Mode & EDT_REVERSE)
	{
        if (EditorBuf.CurLen <= ScreenSize) 
			LeftLen = EditorBuf.CurLen;
        else 
			LeftLen = ScreenSize;

        VTL_ScreenPrintChar(' ', ScreenSize - LeftLen);
        VTL_ScreenPrintMem(&EditorBuf.Buffer[EditorBuf.CurLen - LeftLen], LeftLen);
    }
	else if (MaxLine > 0 && EditorBuf.CurLen > 0) 
	{
        lineno = EditorBuf.CursorPos / GetScreenLineSize();
        if (CurLine >= lineno)
            startline = CurLine - lineno;
        else
            return;

        for (i = startline; i < (startline + GetScreenNumLine()); i++) 
		{
            StartPos = LinePos[i];
            if (i > MaxLine - 1)
			{
                break;
            } 
			else if (i == MaxLine - 1)
			{
                EndPos = EditorBuf.CurLen;
            } 
			else 
			{
                EndPos = LinePos[i + 1];
            }

            if (Editor.Mode & EDT_PASSWORD) 
			{
                for (j = StartPos; j < EndPos; j++)
				{
                    if (j >= Editor.LabelLen)
                        VTL_ScreenPrint('*');
                    else 
                        VTL_ScreenPrint(EditorBuf.Buffer[j]);
                }
            } 
			else
			{
                VTL_ScreenPrintMem(&EditorBuf.Buffer[StartPos], EndPos - StartPos);
            }
        }
    }

    OverWriteScreen();

    if ((Editor.Mode & EDT_CURSOR) && CanShowCursor) 
        ShowCursor(EditorBuf.CursorPos);
    else
        HideCursor();
}

void CHandset::AddEditPos(void)
{
    INT16U  LineSize, NumLine, Temp;
    INT8U   AddStep;
    
    LineSize   = GetScreenLineSize();
    NumLine    = GetScreenNumLine();

    if (EditorBuf.Buffer[EditorBuf.EditPos] >= 0xA1)
	{
        AddStep = 2;
    } else {
        AddStep = 1;
    }
    EditorBuf.EditPos += AddStep;
    SetLinePara();           /* Set Line Parameters */
    
    Temp = EditorBuf.CursorPos / LineSize;

    if (CurLine > PreLine) 
		Temp++;
    if (Temp >= NumLine)
		Temp = NumLine - 1;

    EditorBuf.CursorPos = Temp * LineSize + (EditorBuf.EditPos - LinePos[CurLine]);
}

void CHandset::DecEditPos(INT8U DecStep)
{
    INT16U NumLine, LineSize, Temp;
    
    NumLine  = GetScreenNumLine();
    LineSize = GetScreenLineSize();

    if (EditorBuf.EditPos <= Editor.LabelLen)
		return;
    
    if (EditorBuf.EditPos < DecStep)
		return;

    EditorBuf.EditPos -= DecStep;

    SetLinePara();              /* Set Line Parameters */
    
    Temp = EditorBuf.CursorPos / LineSize;
    if (CurLine < PreLine)
	{
        if (Temp == 0) 
		{
            if (CurLine >= NumLine - 1) 
                Temp = NumLine - 1;
            else 
                Temp = CurLine;
        } 
		else
		{
            Temp--;
        }
    } 
	else 
	{
        if (Temp == 0 && (EditorBuf.EditPos - LinePos[CurLine]) == 0)
		{
            if (CurLine >= NumLine - 1) 
                Temp = NumLine - 1;
            else 
                Temp = CurLine;
        }
    }

    EditorBuf.CursorPos = Temp * LineSize + (EditorBuf.EditPos - LinePos[CurLine]);
}

void CHandset::MovUpEditPos(void)
{
    INT16U NumLine, LineSize;

    NumLine  = GetScreenNumLine();
    LineSize = GetScreenLineSize();
    if (EditorBuf.EditPos <= Editor.LabelLen)
	{
        EditorBuf.EditPos = EditorBuf.CurLen;
        SetLinePara();                                      /* Set Line Parameters */
        if (MaxLine == 0) 
		{
            EditorBuf.CursorPos = 0;
        } 
		else if (MaxLine < NumLine)
		{
            EditorBuf.CursorPos = (MaxLine - 1) * LineSize + (EditorBuf.EditPos - LinePos[CurLine]);
        } 
		else 
		{
            EditorBuf.CursorPos = (NumLine - 1) * LineSize + (EditorBuf.EditPos - LinePos[CurLine]);
        }
    } 
	else 
	{
        if (EditorBuf.Buffer[EditorBuf.EditPos - 1] >= 0xA1)
            DecEditPos(2);
        else
            DecEditPos(1);
    }
}

void CHandset::MovDownEditPos(void)
{
    if (EditorBuf.EditPos < EditorBuf.CurLen) 
        AddEditPos();
    else
	{
        EditorBuf.EditPos = Editor.LabelLen;
        SetLinePara();                                      /* Set Line Parameters */
        EditorBuf.CursorPos = GetInitCursorPos();
    }
}

bool CHandset::AddChar(INT16U ch)
{
    INT8U step;
    
    CheckEditorValid();
    if ((Editor.Mode & EDT_DIGITAL) && Editor.FilterProc == 0) 
	{
        if (ch > '9' || ch < '0') 
            if ((Editor.Mode & EDT_PULS) == 0) 
				return FALSE;
            else if (ch != '+' && ch != '*' && ch != '#')
				return FALSE;
    }

    if (ch >= 0xA100) 
	{
        step = 2;
    } 
	else 
	{
        step = 1;
    }

    if ((GetCurLen() - Editor.LabelLen + step) > GetMaxLen()) 
	{
        ResetConvertKey();
        ActivateStringPrompt_TIME("���ݹ������޷�����", 3);

        return FALSE;
    }

    EditorBuf.CurLen = InsertEditBuf(EditorBuf.Buffer, EditorBuf.CurLen, EditorBuf.EditPos, ch);

    if ((GetCurLen() - Editor.LabelLen) > GetMaxLen()) 
	{
        DelEditBuf(EditorBuf.Buffer, EditorBuf.CurLen, EditorBuf.EditPos + step);
        EditorBuf.CurLen -= step;
        
        ResetConvertKey();
        ActivateStringPrompt_TIME("���ݹ������޷�����", 3);

        return FALSE;
    } 
	else
	{
        AddEditPos();
        return TRUE;
    }
}

bool CHandset::ModifyChar(INT8U ch)
{
    CheckEditorValid();
    if (Editor.Mode & EDT_DIGITAL) 
	{
        if (ch > '9' || ch < '0') 
            if ( (Editor.Mode & EDT_PULS) == 0 ) 
				return FALSE;
            else if (ch != '+' && ch != '*')
				return FALSE;
    }

    if (EditorBuf.EditPos <= Editor.LabelLen)
		return FALSE;

    if (EditorBuf.Buffer[EditorBuf.EditPos - 1] < 0x80)
        EditorBuf.Buffer[EditorBuf.EditPos - 1] = ch;

    return TRUE;
}

void CHandset::InitProc_Editor(void)
{
    INT16U StartPos;
    
    if (Editor.LabelPtr != 0)
	{
        Editor.LabelLen = strlen(Editor.LabelPtr);
        memcpy(EditorBuf.Buffer, (INT8U *)Editor.LabelPtr, Editor.LabelLen);	//�����������
        EditorBuf.CurLen = Editor.LabelLen;	//��ǰ������Ϊ���ⳤ��
    } 
	else
	{
        EditorBuf.CurLen = Editor.LabelLen = 0;	//û�б���ʱ��ǰ����Ϊ0
    }

    if (Editor.InitPtr != 0 && Editor.InitLen)
	{
        memcpy(&EditorBuf.Buffer[EditorBuf.CurLen], Editor.InitPtr, Editor.InitLen);	//�༭����������ĳ�ʼ������
        EditorBuf.CurLen += Editor.InitLen;	//��ǰ�������������ⳤ��+��ʼ�����ݳ���
    }

    if ( EditorBuf.CurLen >= GetScreenSize() ) StartPos = Editor.LabelLen;	//��ǰ���ݳ��ȳ���һ��,>24���ַ�,������������ⳤ�Ⱥ�
    else StartPos = EditorBuf.CurLen;	//����ڵ�ǰ���ȴ�

    EditorBuf.EditPos = StartPos;		//���༭λ����Ϊ���λ��
    
    CheckEditorValid();		//���༭�ַ��Ƿ���Ч
    CurLine = 0;
    SetLinePara();         /* Set Line Parameters */
    EditorBuf.CursorPos = GetInitCursorPos();
    
    if (Editor.Mode & EDT_DIGITAL) 
	{
        EditorBuf.Input = INPUT_DIGITAL;
    } 
	//�������뷨
	else if (Editor.Mode & EDT_CHINESE)
	{
        EditorBuf.Input = INPUT_CHINESE;
    } 
	else 
	{
        EditorBuf.Input = INPUT_UPPER;
    }

    SelInputMode();
    UpdateWin_Editor(TRUE);
    
    memcpy(GetWinStackEnviroment(), &Editor, sizeof(Editor));               /* store editor */
}

void CHandset::DestroyProc_Editor(void)
{
	LockNum();
    HideCursor();
}

void CHandset::SuspendProc_Editor(void)
{
    EDITOR_STRUCT *EnvEditor;
    
    LockNum();
    HideCursor();
    
    EnvEditor = (EDITOR_STRUCT *)GetWinStackEnviroment();

    if (EnvEditor->StorePtr != 0)
        memcpy(EnvEditor->StorePtr, &EditorBuf, sizeof(EditorBuf));         /* store EditorBuf */
}

void CHandset::ResumeProc_Editor(void)
{
    memcpy(&Editor, GetWinStackEnviroment(), sizeof(Editor));               /* resume Editor */
    if (Editor.StorePtr != 0)
	{
        memcpy(&EditorBuf, Editor.StorePtr, sizeof(EditorBuf));             /* resume EditorBuf */
    }
    
    CheckEditorValid();
    SelInputMode();
    SetLinePara();                                                          /* Set Line Parameters */
    UpdateWin_Editor(TRUE);
}

void CHandset::ResumeProc_SelChn(void)
{
    LockNum();
}

void CHandset::SuspendProc_SelChn(void)
{
    HideCursor();
}

void CHandset::DestroyProc_SelChn(void)
{
    HideCursor();
}

void CHandset::HdlKeyProc_SelChn(void)
{
    if( GetKeyMsg(&m_curkey_py) )
	{
		switch(m_curkey_py.key) 
		{
			case KEY_UP:
				{
					if (SCB.status == SEL_PINYIN) 
					{
						if (SCB.tag0 > 0) 
							SCB.tag0--;
					} 
					else
					{
						if (SCB.tag1 > 0) 
							SCB.tag1--;
					}
					UpdateWin_SelChn();
				}
				break;

			case KEY_DOWN:
				{
					INT8U mblen; 
					if (SCB.status == SEL_PINYIN)
					{
						if (SCB.tag0 < SCB.indexlen - 1)
							SCB.tag0++;
					} 
					else 
					{
						mblen = strlen_rom(SCB.py_index[SCB.tag0]->PY_mb) / 2;
						
						if (SCB.tag1 < mblen - 1)
							SCB.tag1++;
					}
					UpdateWin_SelChn();
				}
				break;

			case KEY_ACK:
				{
					INT16U gbcode;
					const char *mb;
    
					if (SCB.status == SEL_PINYIN && SCB.indexlen > 0)
					{
						SCB.tag1   = 0;
						SCB.status = SEL_GBCODE;
						UpdateWin_SelChn();
					} 
					else if (SCB.status == SEL_GBCODE) 
					{
						mb = SCB.py_index[SCB.tag0]->PY_mb;
						gbcode   = mb[2 * SCB.tag1 + 0];
						gbcode <<= 8;
						gbcode  |= (INT8U)mb[2 * SCB.tag1 + 1];
						ExitSelChn(gbcode);
					}
				}
				break;

			case KEY_CLR:
				{
					if (SCB.status == SEL_PINYIN)
					{
						if (!PopKey() || keyword.len == 0)
						{
							ExitSelChn(0);
						}
						else 
						{
							if ((SCB.indexlen = SerachChineseCode(keyword.len, keyword.key, SCB.py_index)) > SIZE_PYINDEX) 
							{
								//ErrExit(ERR_SELCHN_SERACH);
							}
							SCB.tag0 = 0;
							UpdateWin_SelChn();
						}
					} 
					else
					{
						SCB.status = SEL_PINYIN;
						UpdateWin_SelChn();
					}
				}
				break;

			case KEY_CLRALL:
					ExitSelChn(0);
			default:
				Handler_KEY_CHR();
				break;
		}
	}
}

void CHandset::Handler_KEY_CHR(void)
{
    INT16U indexlen;
    
    if (m_curkey_py.key == '#')
	{
        ExitSelChn('#');
    } 
	else if (PushKey(m_curkey_py.key)) 
	{
        if ((indexlen = SerachChineseCode(keyword.len, keyword.key, SCB.py_index)) > SIZE_PYINDEX)
		{
          //  ErrExit(ERR_SELCHN_SERACH);
        }

        if (indexlen > 0)
		{
            SCB.indexlen = indexlen;
            SCB.tag0     = 0;
            SCB.status   = SEL_PINYIN;
            UpdateWin_SelChn();
        }
		else 
		{
            PopKey();
        }
    }
}

void CHandset::InitProc_SelChn(void)
{
    LockNum();
    SCB.tag0   = 0;
    SCB.status = SEL_PINYIN;
    
	if ((SCB.indexlen = SerachChineseCode(keyword.len, keyword.key, SCB.py_index)) > SIZE_PYINDEX) 
	{
    //    ErrExit(ERR_SELCHN_SERACH);
    }

    UpdateWin_SelChn();
}

INT16U CHandset::SerachChineseCode(INT8U indexlen, INT8U *index, const PY_INDEX **result)
{
    INT8U    len, j, ch;
    INT16U   i, ret;
    BOOLEAN  match;
    const char *str;

    if (indexlen > 6 || indexlen == 0)
		return 0;

    ret = 0;

    for (i = 0; i < sizeof(PY_index_all)/sizeof(PY_INDEX); i++) 
	{
        str   = PY_index_all[i].pinyin;
        len   = indexlen;
        j     = 0;
        match = TRUE;

        for (;;) 
		{
            ch = *str++;
            if (ch == '\0') 
			{
                match = FALSE;
                break;
            }

            if (ch == ' ') 
                ch  = 0;
            else
                ch -= ('a' - 1);

            if (TBL_CToG[ch] != index[j++])
			{
                match = FALSE;
                break;
            }

            if (--len == 0) 
			{
                if (str[0] != '\0' && str[1] != '\0') 
				{
                    match = FALSE;
                }
                break;
            }
        }

        if (match) 
		{
            *result++ = &PY_index_all[i];
            ret++;
        }
    }
    return ret;
}

void CHandset::Inform_SelChn(INT16U gbcode)
{
    if (gbcode == '#')
	{
        ActivateSelInputMode();
    } 
	else if (gbcode != 0)
	{
        if (AddChar(gbcode))
            UpdateWin_Editor(TRUE);
    }
}

void CHandset::Inform_ExtKB(INT16U ch)
{
    if (ch != 0) 
	{
        if (AddChar(ch))
            UpdateWin_Editor(TRUE);
    }
}

void CHandset::ActivateSelChn(INT16U initlen, INT8U *initptr, INT8U initkey, void (*informer)(INT16U gbcode))
{
    if (initlen > sizeof(initchar.init)) 
	{
        initptr += (initlen - sizeof(initchar.init));
        initlen  = sizeof(initchar.init);
    }

    initchar.len = initlen;
    memcpy(initchar.init, initptr, initlen);
    keyword.len = 0;
    PushKey(initkey);
    SCB.informer = informer;

    PushWinStack(&WinStackEntry_SelChn);
}

void CHandset::Handler_KEY_CHAR(void)
{
    if (m_curkey_editor.key == '#')
	{
        if (ActivateSelInputMode()) return;
    }

    if (m_curkey_editor.key == '*')
	{
        if (EditorBuf.Input == INPUT_CHINESE) 
		{
            ActivateExtKB(EXTKB_CHINESE, Inform_ExtKB);
            return;
        } 

        if (EditorBuf.Input == INPUT_UPPER || EditorBuf.Input == INPUT_LOWER)
		{
            ActivateExtKB(EXTKB_ASCII, Inform_ExtKB);
            return;
        }
    }
	//090518 ���������뷨�й�ϵ�ľ�����
    if (EditorBuf.Input == INPUT_CHINESE) 
	{
        if (m_curkey_editor.key >= '1' && m_curkey_editor.key <= '9') {
            ActivateSelChn(EditorBuf.EditPos, EditorBuf.Buffer, m_curkey_editor.key, Inform_SelChn);
            return;
        } else if (m_curkey_editor.key == '0') {
	        m_curkey_editor.key = ' ';
        }
    } 

    if (Editor.FilterProc != 0) 
        Editor.FilterProc(&m_curkey_editor.key);

    if (AddChar(m_curkey_editor.key)) 
	{
        if (EditorBuf.Input == INPUT_DIGITAL || EditorBuf.Input == INPUT_CHINESE)
            UpdateWin_Editor(TRUE);
        else 
            UpdateWin_Editor(FALSE);
    }
}

void CHandset::Handler_KEY_CONVERT(void)
{

    if (Editor.FilterProc != 0)
	{
        if (Editor.FilterProc(&m_curkey_editor.key))
		{
            if (AddChar(m_curkey_editor.key))
                UpdateWin_Editor(TRUE);
            return;
        }
    }
    if (ModifyChar(m_curkey_editor.key)) 
        UpdateWin_Editor(FALSE);
}

void CHandset::Handler_KEY_REHIT(void)
{
    if (m_curkey_editor.attrib & ATTR_CHAR) 
	{
        if (Editor.FilterProc != 0)
		{
            if (Editor.FilterProc(&m_curkey_editor.key))
			{
                if (AddChar(m_curkey_editor.key)) 
                    UpdateWin_Editor(TRUE);

                return;
            }
        }

        if (ModifyChar(m_curkey_editor.key)) 
            UpdateWin_Editor(TRUE);
    }
}

//cyh �༭�������Ϣ������
void CHandset::HdlKeyProc_Editor(void)
{
    if ( GetKeyMsg(&m_curkey_editor) ) 
	{
		if (m_curkey_editor.attrib & ATTR_REHIT)
		{
			Handler_KEY_REHIT();
		}    
		else if (m_curkey_editor.attrib & ATTR_CHAR) 
		{
			Handler_KEY_CHAR();
		} 
		else if (m_curkey_editor.attrib & ATTR_CONVERT)
		{
			Handler_KEY_CONVERT();
		} 
		else if (m_curkey_editor.attrib & ATTR_FCN)
		{
			switch(m_curkey_editor.key) 
			{
			case KEY_ACK:
				{
					 CheckEditorValid();
					if (GetCurLen() - Editor.LabelLen >= GetMinLen()) 
					{
						if (Editor.AckProc != 0) 
							Editor.AckProc();
						else
							ExitEditor(); 
					}
				}
				break;

			case KEY_CLR:
				{
					INT8U decstep;
    
					CheckEditorValid();
					if (EditorBuf.CurLen <= Editor.LabelLen)
					{
						ExitEditor();
						return;
					}

					if (EditorBuf.EditPos <= Editor.LabelLen) return;
    
					if ((decstep = DelEditBuf(EditorBuf.Buffer, EditorBuf.CurLen, EditorBuf.EditPos)) > 0) 
					{
						EditorBuf.CurLen -= decstep;
						DecEditPos(decstep);
						if ((Editor.Mode & EDT_REVERSE) && EditorBuf.CurLen <= Editor.LabelLen) {
							ExitEditor();
							return;
						}
					}
					UpdateWin_Editor(TRUE);
				}
				break;

			case KEY_CLRALL:
				{
					EditorBuf.CurLen = EditorBuf.EditPos = Editor.LabelLen;
					if (Editor.Mode & EDT_REVERSE) 
					{
						ExitEditor();
						return;
					}
					
					SetLinePara();    /* Set Line Parameters */
					EditorBuf.CursorPos = GetInitCursorPos();

					UpdateWin_Editor(TRUE);
				}
				break;

			case KEY_UP:
				{
					if (Editor.Mode & EDT_CURSOR) 
					{
						MovUpEditPos();
						UpdateWin_Editor(TRUE);
					}
				}
				break;

			case KEY_DOWN:
				{
					if (Editor.Mode & EDT_CURSOR) 
					{
						MovDownEditPos();
						UpdateWin_Editor(TRUE);
					}
				}
				break;

			case KEY_SPACE:
				UpdateWin_Editor(TRUE);
				break;

			default:
				break;
			}
		}
    }
}

void CHandset::CreateEditor(EDITOR_STRUCT *entry)
{
	memcpy((INT8U *)&Editor, (INT8U *)entry, sizeof(Editor));
    PushWinStack(&WinStackEntry_Editor);
}

void CHandset::ResetEditor(EDITOR_STRUCT *entry)
{
    memcpy((INT8U *)&Editor, (INT8U *)entry, sizeof(Editor));
    InitProc_Editor();
}

void CHandset::ExitEditor(void)
{
    PopWinStack();
    if (Editor.ExitProc != 0) Editor.ExitProc();  //cyh add:?? ��PopWinStack�ˣ����Զ�Ӧ��Editor�Ѿ��ı��ˡ��������Ҫ��PopWinStack֮ǰ����?  ���������edit ������winstack�ϣ��ͻ���� PromptҲ���������⡣
}

INT16U CHandset::GetEditorData(INT8U *ptr)
{
    memcpy(ptr, &EditorBuf.Buffer[Editor.LabelLen], EditorBuf.CurLen - Editor.LabelLen);
    return EditorBuf.CurLen - Editor.LabelLen;
}

/**********************ExtKB**************************************************/
void CHandset::ExitExtKB(INT16U ch)
{
    PopWinStack();

    if (m_extkb.Informer != 0)
		m_extkb.Informer(ch);
}

void CHandset::UpdateWin_SelChn(void)
{
    INT8U pinyinlen, mblen, initlen, linesize;
    INT8U tag, starttag, endtag;
    const char *pinyin, *mb;

    if (SCB.indexlen > 0)
	{
        pinyin = SCB.py_index[SCB.tag0]->pinyin;
    } 
	else
	{
        pinyin = 0;
    }

    pinyinlen  = strlen_rom(pinyin);
    
    linesize   = GetScreenLineSize();
    initlen    = linesize - pinyinlen - 1;     /* 1 for ' ' */

    if (initlen > initchar.len) 
		initlen = initchar.len;

    if (NeedRoundLine(initchar.init + (initchar.len - initlen), initlen))
		initlen--;
    
    VTL_ClearScreen();    /* clear screen */
    VTL_ScreenPrintMem(initchar.init + (initchar.len - initlen), initlen);
    VTL_RelMovCursor(linesize - initlen - pinyinlen);
    for (; pinyinlen > 0; pinyinlen--)
	{
        VTL_ScreenPrint(*pinyin++);
    }
    if (SCB.indexlen > 0)
	{
        if (SCB.status == SEL_PINYIN)
		{
            if (SCB.tag0 >= MAX_SEL) 
                VTL_ScreenPrint(ARROW_LEFT);
            else 
                VTL_ScreenPrint(' ');

            starttag = MAX_SEL * (SCB.tag0 / MAX_SEL);
            endtag   = starttag + (MAX_SEL - 1);

            if (endtag >= SCB.indexlen) 
				endtag = SCB.indexlen - 1;
            
			for (tag = starttag; tag <= endtag; tag++) 
			{
                VTL_ScreenPrint(SCB.py_index[tag]->PY_mb[0]);
                VTL_ScreenPrint(SCB.py_index[tag]->PY_mb[1]);
            }

            if (endtag < SCB.indexlen - 1) 
				VTL_ScreenPrint(ARROW_RIGHT);

            tag = SCB.tag0;
        } 
		else 
		{
            mb    = SCB.py_index[SCB.tag0]->PY_mb;
            mblen = strlen_rom(mb) / 2;

            if (SCB.tag1 >= MAX_SEL)
                VTL_ScreenPrint(ARROW_LEFT);
            else 
                VTL_ScreenPrint(' ');

            starttag = MAX_SEL * (SCB.tag1 / MAX_SEL);
            endtag   = starttag + (MAX_SEL - 1);

            if (endtag >= mblen) 
				endtag = mblen - 1;

            for (tag = starttag; tag <= endtag; tag++)
			{
                VTL_ScreenPrint(mb[2 * tag + 0]);
                VTL_ScreenPrint(mb[2 * tag + 1]);
            }

            if (endtag < mblen - 1)
				VTL_ScreenPrint(ARROW_RIGHT);

            tag = SCB.tag1;
        }
    }
    
    OverWriteScreen();       /* update screen */
    ShowCursor(initlen);     /* show cursor */

    if (SCB.indexlen > 0) 
	{
        tag = linesize + 1 + 2 * (tag % MAX_SEL);
        ReverseBlock(tag, tag + 1);         /* reverse select block */
    }	
}

void CHandset::UpdateWin_ExtKB(void)
{
    VTL_ClearScreen();     /* clear screen */

    if (m_extkb.Mode == EXTKB_CHINESE)
	{
        VTL_ScreenPrintStr(KB_CHINESE);
    }
	else
	{
        VTL_ScreenPrintStr(KB_ASCII);
    }

    OverWriteScreen();     /* overwrite screen */

    if (m_extkb.Mode == EXTKB_CHINESE)
	{
        ReverseBlock(2 * m_extkb.SelPos, 2 * m_extkb.SelPos + 1);
    }
	else
	{
        ReverseBlock(m_extkb.SelPos, m_extkb.SelPos);
    }
}

void CHandset::InitProc_ExtKB(void)
{
    m_extkb.SelPos = 0;
    UpdateWin_ExtKB();
}

void CHandset::ResumeProc_ExtKB(void)
{
    UpdateWin_ExtKB();
}

void CHandset::HdlKeyProc_ExtKB(void)
{
    INT16U SelChar;
    
	if (GetKeyMsg(&m_curkey_extkb))
	{
		switch(m_curkey_extkb.key)
		{
			case KEY_ACK:
				if (m_extkb.Mode == EXTKB_CHINESE) 
				{
					SelChar   = KB_CHINESE[2 * m_extkb.SelPos + 0];
					SelChar <<= 8;
					SelChar  |= (INT8U)KB_CHINESE[2 * m_extkb.SelPos + 1];
				}
				else 
				{
					SelChar   = KB_ASCII[m_extkb.SelPos];
				}
				ExitExtKB(SelChar);
				break;

			case KEY_NAK:
				ExitExtKB(0);
				break;

			case KEY_DOWN:
				m_extkb.SelPos++;
				if (m_extkb.Mode == EXTKB_CHINESE) 
				{
					if (m_extkb.SelPos >= strlen(KB_CHINESE) / 2) 
						m_extkb.SelPos = 0;
				} 
				else 
				{
					if (m_extkb.SelPos >= strlen(KB_ASCII)) 
						m_extkb.SelPos = 0;
				}
				UpdateWin_ExtKB();
				break;

			case KEY_UP:
				if (m_extkb.SelPos > 0) 
					m_extkb.SelPos--;
				else 
				{
					if (m_extkb.Mode == EXTKB_CHINESE) {
						m_extkb.SelPos = strlen(KB_CHINESE) / 2 - 1;
					} else {
						m_extkb.SelPos = strlen(KB_ASCII) - 1;
					}
				}
				UpdateWin_ExtKB();
				break;

			default:
				break;
		}
	}
}

void CHandset::ActivateExtKB(INT8U Mode, void (*Informer)(INT16U ch))
{
	WINSTACKENTRY_STRUCT WinStackEntry /*= {InitProc_ExtKB, 0, 0, ResumeProc_ExtKB, HdlKeyProc_ExtKB}*/;
	WinStackEntry.DestroyProc = 0;
	WinStackEntry.HdlKeyProc = HdlKeyProc_ExtKB;
	WinStackEntry.InitProc = InitProc_ExtKB;
	WinStackEntry.ResumeProc = ResumeProc_ExtKB;
	WinStackEntry.SuspendProc = 0;
	
    m_extkb.Mode     = Mode;
    m_extkb.Informer = Informer;
    PushWinStack(&WinStackEntry);
}

//cyh add ��ȡ�˵��� ����  ����ʾ
void CHandset::ReadRecord(void)
{
    INT16S recnum;
    
    if (fmenuwin.maxrecord == 0) 
        recnum = -1;
    else 
        recnum = fmenuwin.currecord;
    
	if ((record.len = fmenuwin.readproc(recnum, record.data)) > sizeof(record.data))
        return;
    
	InitTextDply(record.data, record.len);
}

void CHandset::CreateFMenuWin(INT16U initrecord, INT16U maxrecord, INT16U (*readproc)(INT16S num, INT8U *ptr))
{
    fmenuwin.currecord = initrecord;
    fmenuwin.maxrecord = maxrecord;
    fmenuwin.readproc  = readproc;
    ReadRecord();
}

INT16U CHandset::StoreFMenuWin(INT8U *storeptr)
{
    memcpy(storeptr, &fmenuwin, sizeof(fmenuwin));
    return sizeof(fmenuwin);
}

void CHandset::ReasumeFMenuWin(INT8U *storeptr)
{
    memcpy(&fmenuwin, storeptr, sizeof(fmenuwin));
    ReadRecord();
}

void CHandset::PageUpFMenuWin(void)
{
    if (fmenuwin.maxrecord == 0) return;

    if (!PageUpText()) 
	{
        if (fmenuwin.currecord == 0) 
            fmenuwin.currecord = fmenuwin.maxrecord - 1;
        else
            fmenuwin.currecord--;
        ReadRecord();
    }
}

void CHandset::PageDownFMenuWin(void)
{
    if (fmenuwin.maxrecord == 0) return;

    if (!PageDownText()) 
	{
        if (++fmenuwin.currecord >= fmenuwin.maxrecord) fmenuwin.currecord = 0;
        ReadRecord();
    }
}

INT16S CHandset::GetFMenuCurRecord(void)
{
    if (fmenuwin.maxrecord == 0) 
        return -1;
    else 
        return fmenuwin.currecord;
}

/*********************HstCtl***************************************************/
void CHandset::TurnonHst()
{
	SendHstFrame(CMD_PowerOn, NULL, 0);
}

void CHandset::TurnoffHst()
{
	SendHstFrame(CMD_PowerOff, NULL, 0);
}

void CHandset::TurnonBKLight()
{
	SendHstFrame(CMD_LightOn, NULL, 0);
}

void CHandset::TurnoffBKLight()
{
	SendHstFrame(CMD_PowerOff, NULL, 0);
}

void CHandset::CancelRing()
{
	SendHstFrame(CMD_RingingCancle, NULL, 0);
}

void CHandset::RequestRing(char data, int len)
{
	SendHstFrame(CMD_RingingType, &data, len);
}

void CHandset::CtlKeyPrompt(char data, int len)	
{
	SendHstFrame(CMD_KeysTone, &data, len);
}

void CHandset::InitHstIndicator()
{
	m_indicator.voltage = 5;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}
	
void CHandset::TurnonSMIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.sm = 1;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::TurnoffSMIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.sm = 0;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::TurnonArrowIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.arrow = 1;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::TurnoffArrowIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.arrow = 0;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::CtlSignalIndicator(INT8U level)
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.signal = level;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::CtlVoltageIndicator(INT8U level)
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.voltage = level;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::TurnonTalkIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.talk = 1;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::TurnoffTalkIndicator()
{
	memset((INT8U *)&m_indicator, 0x0f, sizeof(m_indicator));
	m_indicator.talk = 0;
	SendHstFrame(CMD_AuxShow, (char *)&m_indicator, sizeof(m_indicator));
}

void CHandset::CtlHstContrast(INT8U level)
{
	char tempdata;
	tempdata = level + 0x80;
	SendHstFrame(CMD_AdContrast, &tempdata, 1);
}

void CHandset::ChkHst()
{
	SendHstFrame(CMD_ChkHandle, NULL, 0);
}

/*********************ItemMenu***************************************************/
void CHandset::GetCurItemMenu(void)
{
    memcpy(&ItemMenu_cur, GetMenuWinEnviroment(), sizeof(ItemMenu_cur));
}

INT16U CHandset::StoreProc_ItemMenu(INT8U *StorePtr)
{//cyh add ���浱ǰ�˵����ԵĽṹ�壬���ز˵����Խṹ����ռ�õ��ֽ���
    memcpy(StorePtr, &ItemMenu_cur, sizeof(ItemMenu_cur));
    return sizeof(ItemMenu_cur);
}

INT16U CHandset::MaxRecordProc_ItemMenu(void)
{//cyh add��ȡ��ǰ�˵��� �м����Ӳ˵�
    GetCurItemMenu();
    return ItemMenu_cur.NumChild;
}

INT16U CHandset::ReadRecordProc_ItemMenu(INT16S num, INT8U *ptr)
{//cyh add: ��ȡ��ǰ�˵��ĵ�num���Ӳ˵��������˵����ݿ�����ptr��ָ�ĵ�ַ�����ؿ������ֽ���
	
    GetCurItemMenu();

    if (num == -1) 
		return 0;
    else 
	{
        if (num >= ItemMenu_cur.NumChild)
			return 0;

        memcpy(ptr, ItemMenu_cur.ChildItem[num].ContentStr, strlen(ItemMenu_cur.ChildItem[num].ContentStr));

        return strlen(ItemMenu_cur.ChildItem[num].ContentStr);
    }
}

void CHandset::Handler_KEY_ACK_ItemMenu(void)
{
    INT16S num;
    void (*EntryProc)(void);
    
    GetCurItemMenu();
    num = GetMenuWinRecordNum();  //cyh add ��ȡ��ǰ�˵���������
   
	if (num == -1 || num >= ItemMenu_cur.NumChild)
		return;

    if (ItemMenu_cur.ChildItem[num].Attrib == ITEM_FCN)
	{//cyh add:���Ҫ������ǹ��ܲ˵�
        if ((EntryProc = ItemMenu_cur.ChildItem[num].EntryProc) != 0) EntryProc();
    }
	else
	{//cyh add:���Ҫ����Ľ�����������ʾ�˵�
        ActivateItemMenu(&ItemMenu_cur.ChildItem[num]);
    }
}

void CHandset::ActivateItemMenu_Init(ITEMMENU_STRUCT *ItemMenuEntry, INT16U (*InitRecordProc)(void))
{
	//cyh add: ItemMenuEntry:Ҫ���صĲ˵�  ,InitRecordProc:�˵����غ���ʾ�ļ�¼�� ������
    INT8U attrib;
    MENUWIN_STRUCT MenuWinEntry; 

	memcpy(&ItemMenu_cur, ItemMenuEntry, sizeof(ItemMenu_cur));

	MenuWinEntry.Handler_KEY_ACK = Handler_KEY_ACK_ItemMenu;  //cyh add:�ڲ˵��ϣ�OK���Ĵ�����.��Ȼ����ֱ��ִ�У����Ǻ��������յ�OK���󣬻�����������
	MenuWinEntry.Handler_KEY_FCN = 0;
	MenuWinEntry.Handler_KEY_NAK = 0;
	MenuWinEntry.Handler_KEY_SEL = 0;
	MenuWinEntry.MaxRecordProc = MaxRecordProc_ItemMenu;
	MenuWinEntry.ReadRecordProc = ReadRecordProc_ItemMenu;
	MenuWinEntry.StoreProc = StoreProc_ItemMenu;
	
    if (ItemMenu_cur.Attrib == ITEM_FMENU) 
        attrib = WIN_FMENU;
    else if (ItemMenu_cur.Attrib == ITEM_LMENU) 
        attrib = WIN_LMENU;
    else 
        return;

    MenuWinEntry.WinAttrib      = attrib;
    MenuWinEntry.InitRecordProc = InitRecordProc;
    ActivateMenuWin(&MenuWinEntry);
}

void CHandset::ActivateItemMenu(ITEMMENU_STRUCT *ItemMenuEntry)
{//cyh add: ����˵�����
    ActivateItemMenu_Init(ItemMenuEntry, 0);
}

/**********************Keyboard***********************************************/
void CHandset::SendCurKeyMsg(void)
{
	INT8U buf[2];
	buf[0] = m_curkey_kb.attrib;
	buf[1] = m_curkey_kb.key;

	keymsg_stack.push((INT8U *)buf, 2);

	if (0 != keymsg_stack.getcount())	//�����ջ�������������
	{		
		g_hsttm = GetTickCount();
	}
}

bool CHandset::GetDEFKey(INT8U inkey)
{
    if (inkey >= '0' && inkey <= '9')	//���������
	{
        m_curkey_kb.attrib = ATTR_CHAR;
        m_curkey_kb.key    = inkey;
    } 
	else if (inkey == KEY_STAR_ || inkey == KEY_JING_) // * #
	{
        if (!numlocked) return false;	//������ַ�����,�򷵻�
        else {
            m_curkey_kb.attrib = ATTR_CHAR;
            if (inkey == KEY_STAR_) m_curkey_kb.key = '*';
            else m_curkey_kb.key = '#';
        }
    } 
	else if (inkey >= 'c' && inkey <= 'x') //����ǹ��ܼ�
	{
        m_curkey_kb.attrib = ATTR_FCN;
        m_curkey_kb.key    = FCNKEYCOLLECT[inkey - 'c'];
    } 
	else 
	{
        return false;
    }
    return true;
}

void CHandset::HdlRehitKey(void)
{
    INT8U orgkey;

    orgkey = prekey;
    prekey = KEY_NULL;
        
    if (orgkey == KEY_JING_) 
	{
        m_curkey_kb.attrib = ATTR_FCN;
        m_curkey_kb.key    = KEY_NUMTOGGLE;
    } else {
        if (!GetDEFKey(orgkey)) return;
        if (m_curkey_kb.key == KEY_CLR) m_curkey_kb.key = KEY_CLRALL;
        else if (m_curkey_kb.key == KEY_UP) m_curkey_kb.key = KEY_PAGEUP;
        else if (m_curkey_kb.key == KEY_DOWN) m_curkey_kb.key = KEY_PAGEDOWN;
        else m_curkey_kb.attrib |= ATTR_REHIT;
    } 
    SendCurKeyMsg();
}

bool CHandset::GetConvertKey(void)
{
    INT8U tag;
    
    if (recvkey >= '0' && recvkey <= '9') 
		tag = recvkey - '0';
    else if (recvkey == KEY_STAR_) 
		tag = 10;
    else 
		return FALSE;

	_SetTimer(&g_objTimerMng, HST_CONVERT_TIMER, 2000, G_TmHstConver);

	if (recvkey == prekey) 
	{
		m_curkey_kb.attrib = ATTR_CONVERT;
	}
    else
	{
        hitnum        = 1;
        prekey        = recvkey;
        m_curkey_kb.attrib = ATTR_CHAR;
    }

    hitnum++;
    if (hitnum > CONVERTKEY[tag][0]) 
		hitnum = 1;

    m_curkey_kb.key = CONVERTKEY[tag][hitnum];

    return TRUE;
}

void CHandset::ParseKeyBoard(char v_szData)
{
	recvkey = v_szData;

	if (recvkey >= 'c' && recvkey <= 'x') //����ǹ��ܼ�
	{
		if (recvkey == KEY_REHIT_)
		{
			HdlRehitKey();
		} 
		else 
		{
			if (recvkey != KEY_LOOSEN_)
			{
				prekey = recvkey;
			}

			if (GetDEFKey(recvkey)) 
				SendCurKeyMsg();
		}

		return;
	}
    
    //090523 hxd 
	if (numlocked) 
	{	
		if ((recvkey >= '0' && recvkey <= '9') || recvkey == 'a' || recvkey == 'b')		//���������, * , #
		{
			if (recvkey == KEY_STAR_) //����������'*'
			{
				GetConvertKey();	//���ּ��̵�'*'���Ա�ʾ"*" �� "+"  ,���������α�ʾ"+"
			} 
			else 
			{
				prekey = recvkey;
				if (!GetDEFKey(recvkey)) return;
			}
			SendCurKeyMsg();
		} 
	}
	else	//������ַ���
	{
		if (recvkey == KEY_STAR_ || recvkey == KEY_JING_) 
		{
			prekey = recvkey;
			if (recvkey == KEY_STAR_) 
			{
				m_curkey_kb.attrib = ATTR_CHAR;
				m_curkey_kb.key    = '*';
			} else 
			{
				m_curkey_kb.attrib = ATTR_CHAR;
				m_curkey_kb.key    = '#';
			}
		} 
		else 
		{
			if (!GetConvertKey()) return;
			if ((!uppercase) && (m_curkey_kb.key >= 'A' && m_curkey_kb.key <= 'Z')) {
				m_curkey_kb.key = 'a' + (m_curkey_kb.key - 'A');
			}
		}
		SendCurKeyMsg();
	}  
}

void CHandset::ToggleCase(void)	//��Сд�л�
{
    uppercase = !uppercase;
}

void CHandset::ToggleUpperCase(void)	//��д
{
    uppercase = TRUE;
}

void CHandset::ToggleLowerCase(void)	//Сд
{
    uppercase = FALSE;
}

void CHandset::UnlockNum(void)	//��Ϊ�ַ�����
{
    numlocked = false;
    uppercase = true;
}

void CHandset::LockNum(void)	//��Ϊ���ּ���
{
    numlocked = true;
}

void CHandset::ResetConvertKey(void)
{
    prekey        = KEY_NULL;
    m_curkey_kb.attrib = ATTR_FCN;
    m_curkey_kb.key    = KEY_SPACE;
    SendCurKeyMsg();

	_KillTimer(&g_objTimerMng, HST_CONVERT_TIMER);
}

void CHandset::InitKeyBoard(void)
{
    prekey     = KEY_NULL;
    numlocked  = true;
    uppercase  = true;
}

/******************LMenuWin******************************************************/
void CHandset::ReadRecord_Up(void)
{
    INT16U indicator, i;
    
    if (lmenuwin.maxrecord == 0) return;

    indicator = lmenuwin.indicator;
    if (lmenuwin.maxrecord <= MAX_RECORD) 
	{
        if (indicator == 0) indicator = lmenuwin.maxrecord - 1;
        else indicator--;

        lmenuwin.indicator = indicator;
    }
	else if (indicator > 0)
	{
        lmenuwin.indicator = indicator - 1;
    } 
	else 
	{
        for (i = 0; i <= MAX_RECORD - 2; i++)
            memcpy(&record_LMenu[MAX_RECORD - 1 - i], &record_LMenu[MAX_RECORD - 2 - i], sizeof(RECORD_STRUCT));

        if (lmenuwin.startpos == 0) lmenuwin.startpos = lmenuwin.maxrecord - 1;
        else lmenuwin.startpos--;

        record_LMenu[0].len = lmenuwin.readproc(lmenuwin.startpos, record_LMenu[0].data);
    }
}

void CHandset::ReadRecord_Down(void)
{
    INT16U indicator, i, tag;
    
    if (lmenuwin.maxrecord == 0) return;

    indicator = lmenuwin.indicator;
    if (lmenuwin.maxrecord <= MAX_RECORD) 
	{
        indicator++;
        if (indicator >= lmenuwin.maxrecord) indicator = 0;
        lmenuwin.indicator = indicator;
    }
	else if (indicator < MAX_RECORD - 1) 
	{
        lmenuwin.indicator = indicator + 1;
    }
	else
	{
        for (i = 0; i <= MAX_RECORD - 2; i++) 
            memcpy(&record_LMenu[i], &record_LMenu[i + 1], sizeof(RECORD_STRUCT));  //cyh add: readproc����ȡһ����¼����䵽record_LMenu�����һ��λ��
        
        tag = lmenuwin.startpos + 1;
        if (tag >= lmenuwin.maxrecord) tag = 0;

        lmenuwin.startpos = tag;
        tag += (MAX_RECORD - 1);
        if (tag >= lmenuwin.maxrecord) tag -= lmenuwin.maxrecord;

        record_LMenu[MAX_RECORD - 1].len = lmenuwin.readproc(tag, record_LMenu[MAX_RECORD - 1].data);
    }
}

void CHandset::ReadRecord_Init(void)
{
    INT16U i, j, tag;
    
    if (lmenuwin.maxrecord == 0) return;
    if (lmenuwin.maxrecord < MAX_RECORD) j = lmenuwin.maxrecord;
    else j = MAX_RECORD;
    
    tag = lmenuwin.startpos;
    for (i = 0, j--; i <= j; i++) 
	{
        record_LMenu[i].len = lmenuwin.readproc(tag, record_LMenu[i].data);
        if (++tag >= lmenuwin.maxrecord) tag = 0;
    }
}

void CHandset::UpdateWin_LMenuWin(void)
{
    INT16U i, j, linesize;
    
    VTL_ClearScreen();
    if (lmenuwin.maxrecord == 0)
	{
        if ((record_LMenu[0].len = lmenuwin.readproc(-1, record_LMenu[0].data)) == 0)
		{
            VTL_ScreenPrintStr("�޼�¼");
        }
		else
		{
            VTL_ScreenPrintMem(record_LMenu[0].data, record_LMenu[0].len);
        }
        OverWriteScreen();
    } 
	else 
	{
        linesize = GetScreenLineSize();
        if (lmenuwin.maxrecord < MAX_RECORD) j = lmenuwin.maxrecord;
        else j = MAX_RECORD;

        for (i = 0, j--; i <= j; i++)		
		{
            VTL_AbsMovCursor(i * linesize);  //cyh add ������ƶ�������

            if (record_LMenu[i].len > linesize) 
			{
                VTL_ScreenPrintMem(record_LMenu[i].data, linesize - 1);
                VTL_ScreenPrint(0x80);  //cyh add:????
            }
			else
			{
                VTL_ScreenPrintMem(record_LMenu[i].data, record_LMenu[i].len);
            }
        }

        OverWriteScreen();
        ReverseLine(lmenuwin.indicator);
    }
}

void CHandset::CreateLMenuWin(INT16U initrecord, INT16U maxrecord, INT16U (*readproc)(INT16S num, INT8U *ptr))
{
    lmenuwin.readproc  = readproc;
    lmenuwin.maxrecord = maxrecord;
    lmenuwin.startpos  = initrecord;
    lmenuwin.indicator = 0;
    ReadRecord_Init();
    UpdateWin_LMenuWin();
}

INT16U CHandset::StoreLMenuWin(INT8U *storeptr)
{
    memcpy(storeptr, &lmenuwin, sizeof(lmenuwin));
    return sizeof(lmenuwin);
}

void CHandset::ResumeLMenuWin(INT8U *storeptr)
{
    memcpy(&lmenuwin, storeptr, sizeof(lmenuwin));
    ReadRecord_Init();
    UpdateWin_LMenuWin();
}

void CHandset::PageUpLMenuWin(void)
{
    ReadRecord_Up();
    UpdateWin_LMenuWin();
}

void CHandset::PageDownLMenuWin(void)
{
    ReadRecord_Down();
    UpdateWin_LMenuWin();
}

INT16S CHandset::GetLMenuCurRecord(void)
{
    INT16U tag;
    
    if (lmenuwin.maxrecord == 0) return -1;
    tag = lmenuwin.startpos + lmenuwin.indicator;
    if (tag >= lmenuwin.maxrecord) tag -= lmenuwin.maxrecord;
    return tag;
}

/**********************MenuWin************************************************/
void CHandset::CreateMenuWin(INT16U initrecord, INT16U maxrecord)
{//cyh add: initrecord: ������ʾ�ڽ����ϵĲ˵���������  maxrecord:�Ӳ˵��ĸ���
    WinEntry.attrib     = MenuWin_cur.WinAttrib;
    WinEntry.maxrecord  = maxrecord;
    WinEntry.initrecord = initrecord;
    WinEntry.readproc   = MenuWin_cur.ReadRecordProc;
    CreateWin(&WinEntry);
}

void CHandset::InitProc_MenuWin(void)
{//cyh add: 
    INT8U  *StorePtr;
    INT16U StoreLen, MaxRecord, InitRecord;
    
    StorePtr = GetWinStackEnviroment();
    if (MenuWin_cur.StoreProc != 0) 
        StoreLen = MenuWin_cur.StoreProc(StorePtr + 2);    //cyh add:Ӧ���Ǳ���ItemMenu             /* store menuwin enviroment */
    else 
        StoreLen = 0;
    WriteWordByPointer(StorePtr, StoreLen);        /* store menuwin enviroment size */
    StorePtr += (2 + StoreLen);
    
    MaxRecord = MenuWin_cur.MaxRecordProc();
    WriteWordByPointer(StorePtr, MaxRecord);       /* store maxrecord */
    StorePtr += 2;
    memcpy(StorePtr, &MenuWin_cur, sizeof(MenuWin_cur));   /* store menuwin structure */
    StorePtr += sizeof(MenuWin_cur);
    
    if (MenuWin_cur.InitRecordProc != 0) InitRecord = MenuWin_cur.InitRecordProc();
    else InitRecord = 0;
    CreateMenuWin(InitRecord, MaxRecord);
    StoreLen = StoreWin(StorePtr + 2);         /* store window structure */
    WriteWordByPointer(StorePtr, StoreLen);    /* store window structure size */
}

void CHandset::SuspendProc_MenuWin(void)
{
    INT8U *StorePtr;
    INT16U StoreLen;
    
    StorePtr = GetWinStackEnviroment();
    StoreLen = ReadWordByPointer(StorePtr);
    StoreWin(StorePtr + 2 + StoreLen + 2 + sizeof(MenuWin_cur) + 2);

	//cyh add:environment����������� 2B+ItemMenu+2B(MaxRecord)+MenuWin+2B+1B(��ǰ�˵�����)+�˵�
}

void CHandset::ResumeProc_MenuWin(void)
{
    INT8U   *StorePtr;
    INT16U  PreMaxRecord, CurMaxRecord, StoreLen;
    
    StorePtr  = GetWinStackEnviroment();
    StoreLen  = ReadWordByPointer(StorePtr);
    StorePtr += (2 + StoreLen);
    
    PreMaxRecord = ReadWordByPointer(StorePtr);                     /* resume maxrecord */
    StorePtr    += 2;
    memcpy(&MenuWin_cur, StorePtr, sizeof(MenuWin_cur));                    /* resume menuwin */
    StorePtr    += (sizeof(MenuWin_cur) + 2);

    if ((CurMaxRecord = MenuWin_cur.MaxRecordProc()) == 0)  //cyh add: ��MaxRecordProc����ָ�ItemMenu
	{
        PopWinStack();
        return;
    }

    if (CurMaxRecord != PreMaxRecord) 
	{
        CreateMenuWin(0, CurMaxRecord);
        StoreWin(StorePtr);                                         /* store window */
    } 
	else 
	{
        ResumeWin(StorePtr);                                        /* resume window */
    }

}

void CHandset::HdlKeyProc_MenuWin(void)
{//cyh add:  �����˵������µİ�������������Ӧ(����OK��OK����MenuWin.Handler_KEY_ACK��Handler_KEY_ACK_ItemMenu����)

	if (GetKeyMsg(&m_curkey_menuwin)) 
	{
		switch (m_curkey_menuwin.key)
		{
			case KEY_UP:
			case KEY_DOWN:
				{
					if (m_curkey_menuwin.key == KEY_UP)
						PageUpWin();
					else
						PageDownWin();

					if (MenuWin_cur.Handler_KEY_SEL != 0)
						MenuWin_cur.Handler_KEY_SEL();
				}
				break;

			case KEY_ACK:
				{
					if (MenuWin_cur.MaxRecordProc() == 0) 
						PopWinStack();
					else if (MenuWin_cur.Handler_KEY_ACK != 0) 
						MenuWin_cur.Handler_KEY_ACK();
				}
				break;

			case KEY_NAK:
				{
					if (MenuWin_cur.Handler_KEY_NAK != 0) 
						MenuWin_cur.Handler_KEY_NAK();

					PopWinStack();
				}
				break;

			case KEY_FCN:
				{
					if (MenuWin_cur.MaxRecordProc() == 0) 
						PopWinStack();
					else if (MenuWin_cur.Handler_KEY_FCN != 0)
						MenuWin_cur.Handler_KEY_FCN();
				}
				break;

			default:
				break;
		}
	}
}

void CHandset::RefreshMenuWin(void)
{
    ResumeProc_MenuWin();
}

void CHandset::PopMenuWin(void)
{
    PopWinStack();
}

void CHandset::P_TmHstConvert()
{
	ResetConvertKey();
}

void CHandset::ActivateMenuWin(MENUWIN_STRUCT *MenuWinEntry)
{//cyh add: �����˵����ڵĹ��ܺ�����ע������û�п���ItemMenu. ������ֱ�ӵ��ô˺�������˵�ʱ��ʹ�õ���Ȼ��֮ǰ��ItemMenu����˼�����environment����ItemMenu�����ڿ���Ӧ��Resume����ʱҪע��
	
	memcpy(&MenuWin_cur, MenuWinEntry, sizeof(MenuWin_cur));  //cyh add: �ȿ���������˵��Ĺ��ܺ������ټ���˵���ʾ����

	WINSTACKENTRY_STRUCT WinStackEntry;
	WinStackEntry.DestroyProc = 0;
	WinStackEntry.HdlKeyProc = HdlKeyProc_MenuWin;
	WinStackEntry.InitProc = InitProc_MenuWin;
//	WinStackEntry.InitProc = Convert_InitProc_MenuWin(Helper_InitProc_MenuWin);
	WinStackEntry.ResumeProc = ResumeProc_MenuWin;
	WinStackEntry.SuspendProc = SuspendProc_MenuWin;

    
    PushWinStack(&WinStackEntry);
}

INT8U *CHandset::GetMenuWinEnviroment(void)
{
    return GetWinStackEnviroment() + 2;
}

INT16S CHandset::GetMenuWinRecordNum(void)
{//cyh add����ȡ��ǰ�Ĳ˵���������ţ���0��ʼ
    return GetCurWinRecord();
}

/*********************Prompt***************************************************/
void CHandset::ExitPrompt(void)
{
    PopWinStack();

    if (Prompt.ExitProc != 0)
	{
        if (Prompt.Mode & PROMPT_ACKINFORM)
		{
            if (m_curkey_prompt.key == KEY_ACK)
			{
				Prompt.ExitProc();
			}
        }
		else
		{
            Prompt.ExitProc();
        }
    }
}

void CHandset::InitProc_Prompt(void)
{
    WinEntry_Prompt.attrib  = WIN_TEXT;
    WinEntry_Prompt.textptr = Prompt.TextPtr;
    WinEntry_Prompt.textlen = Prompt.TextLen;
	g_dwPromptTime = Prompt.LiveTime;  //cyh add:add
	g_dwTmPromptStart = GetTickCount();  //cyh add:add

    CreateWin(&WinEntry_Prompt);
    memcpy(GetWinStackEnviroment(), &Prompt, sizeof(Prompt));
}

void CHandset::DestroyProc_Prompt(void)
{
    //Prompt.LiveTime = 0;  //cyh add:del
//	ActivateLowestWinStack();
//	ExitPrompt();	//�����ջ�е��ϲ�û����Դ,�ٴ����ٸý���,ֱ���ײ�λ��
}

void CHandset::SuspendProc_Prompt(void)
{
	g_dwTmPromptStart = 0;  //cyh add:add
	g_dwPromptTime = 0;  //cyh add:add

    if (Prompt.LiveTime > 0)
        ExitPrompt();
    else
        StoreWin(GetWinStackEnviroment() + sizeof(Prompt));  
}

void CHandset::ResumeProc_Prompt(void)
{
	memcpy(&Prompt, GetWinStackEnviroment(), sizeof(Prompt));
	g_dwTmPromptStart = GetTickCount();  //cyh add:add
	g_dwPromptTime = Prompt.LiveTime;  //cyh add:add
	ResumeWin(GetWinStackEnviroment() + sizeof(Prompt));   
}

void CHandset::HdlKeyProc_Prompt(void)//------------cyh add:mod-----------
{
	static BOOL sta_bExit = FALSE;
	
	if (g_dwTmPromptStart && GetTickCount()-g_dwTmPromptStart>g_dwPromptTime*1000 && !(Prompt.Mode&PROMPT_NOT_RETURN) )
	{//���Promptʱ�䵽�����ǿ����Զ����ص���ʾ��
		sta_bExit = TRUE;
	}

	if (GetKeyMsg(&m_curkey_prompt) || sta_bExit) 
	{
		if (Prompt.Mode&PROMPT_NOT_RETURN)
		{//����ǲ��Զ����ص���ʾģʽ������Ҫ�ֶ�PopWinStack�����˳����Prompt������U�������ȵط�
			sta_bExit = FALSE;
			return;
		}

		if (sta_bExit)
		{
			sta_bExit = FALSE;
			g_dwPromptTime = 0;
			g_dwTmPromptStart = 0;
			m_curkey_prompt.key = KEY_NAK;  //cyh add:ģ��һ����ʱ�˳���Ϣ
		}

		switch (m_curkey_prompt.key)
		{
			case KEY_ACK:
				{
					g_dwTmPromptStart = 0;
					g_dwPromptTime = 0;

					if (Prompt.ExitKeyProc != 0)
					{
						Prompt.ExitKeyProc();
					}

 					ExitPrompt();
				}
				break;

			case KEY_NAK:
				{
					g_dwTmPromptStart = 0;
					g_dwPromptTime = 0;

					if (Prompt.ExitProc != 0)
					{
						Prompt.ExitProc();
					}

 					ExitPrompt();
				}

				break;
			case KEY_UP:
				{
					PageUpWin();
				}
				break;
			case KEY_DOWN:				
				{
					PageDownWin();
				}
				
				break;
			default:
				break;
		}
	}
}

void CHandset::ActivatePrompt(PROMPT_STRUCT *PromptEntry)
{
//	WINSTACKENTRY_STRUCT WinStackEntry_Prompt; /*= {InitProc_Prompt, DestroyProc_Prompt, SuspendProc_Prompt, ResumeProc_Prompt, HdlKeyProc_Prompt}*/;
//	WinStackEntry_Prompt.DestroyProc = DestroyProc_Prompt;
//	WinStackEntry_Prompt.HdlKeyProc = HdlKeyProc_Prompt;
//	WinStackEntry_Prompt.InitProc = InitProc_Prompt;
//	WinStackEntry_Prompt.ResumeProc = ResumeProc_Prompt;
//	WinStackEntry_Prompt.SuspendProc = SuspendProc_Prompt;
	
// 	g_dwPromptTime = PromptEntry->LiveTime;
// 	g_dwTmPromptStart = GetTickCount();
    memcpy(&Prompt, PromptEntry, sizeof(Prompt));
    PushWinStack(&WinStackEntry_Prompt);
}

void CHandset::ActivateStringPrompt(char *textstr)
{
    ActivateStringPrompt_TIME(textstr, 3);
}

void CHandset::ActivateStringPrompt_TIME(char *textstr, INT8U delaytime)
{
	if (delaytime==0)
	{//cyh add:add
		delaytime = 3;
	}

    PromptEntry.Mode        = 0;
    PromptEntry.LiveTime    = delaytime;
    PromptEntry.TextPtr     = (INT8U *)textstr;
    PromptEntry.TextLen     = strlen(textstr);
    PromptEntry.ExitProc    = 0;
    PromptEntry.ExitKeyProc = 0;
    ActivatePrompt(&PromptEntry);
}

void CHandset::ActivateStringPrompt_TIME_KEY(char *textstr,INT8U delaytime,void(*KeyProc)(void))
{
	if (delaytime==0)
	{
		delaytime = 3;
	}

    PromptEntry.Mode        = 0;
    PromptEntry.LiveTime    = delaytime;
    PromptEntry.TextPtr     = (INT8U *)textstr;
    PromptEntry.TextLen     = strlen(textstr);
	PromptEntry.ExitProc    = KeyProc;//0;
    PromptEntry.ExitKeyProc = 0;//KeyProc;
    ActivatePrompt(&PromptEntry);
}

void CHandset::ActivateSelPrompt_TIME(void *textptr, INT16U textlen, void (*SelProc)(void), INT8U delaytime)
{
	if (0==delaytime)
	{
		delaytime=9;
	}
    PromptEntry.Mode        = PROMPT_ACKINFORM;
    PromptEntry.LiveTime    = delaytime;
    PromptEntry.TextPtr     = (INT8U *)textptr;
   
	if (textlen == 0)
	{
        PromptEntry.TextLen = strlen((char *)textptr);
	}
    else
	{
        PromptEntry.TextLen = textlen;
	}

    PromptEntry.ExitProc    = SelProc;
    PromptEntry.ExitKeyProc = 0;
    ActivatePrompt(&PromptEntry);
}

void CHandset::ActivateSelPrompt(void *textptr, INT16U textlen, void (*SelProc)(void))
{
    ActivateSelPrompt_TIME(textptr, textlen, SelProc, 10);
}

/*******************Screen*****************************************************/
INT8U CHandset::GetScreenSize(void)
{
	return SCREENSIZE;
}

INT8U CHandset::GetScreenLineSize(void)
{
	return LINESIZE;
}

INT8U CHandset::GetScreenNumLine(void)
{// cyh add: ��ȡ��Ļ����ʾ������
	return NUMLINE;
}

void CHandset::VTL_ClearScreen(void)
{
	screen.l = screen.cursor = 0;
	memset(screen.data, ' ', sizeof(screen.data));
}

void CHandset::VTL_ScreenPrint(INT8U ch)
{
	if ( CheckScreenValid() ) 
	{
		if (ch == '\n') FilterChar('\r'); 
		FilterChar(ch);
	}
}

void CHandset::VTL_ScreenPrintMem(INT8U *ptr, INT8U len)
{
	for (; len > 0; len--) VTL_ScreenPrint(*ptr++);
}

void CHandset::VTL_ScreenPrintStr(char *str)
{
	INT8U len;
   
	len = strlen(str);
	for (; len > 0; len--) VTL_ScreenPrint(*str++);
}

void CHandset::VTL_ScreenPrintChar(INT8U ch, INT8U len)
{//cyh add: ����Ļ�ϴ�ӡ�� len �� ch �ַ�
	for (; len > 0; len--) VTL_ScreenPrint(ch);
}

void CHandset::VTL_AbsMovCursor(INT8U pos)
{
	if (pos > SCREENSIZE) pos = SCREENSIZE;
	screen.cursor = pos;
	if (screen.cursor > screen.l) screen.l = screen.cursor;
}

void CHandset::VTL_RelMovCursor(INT8U pos)
{
	VTL_AbsMovCursor(screen.cursor + pos);
}
	
void CHandset::OverWriteScreen()
{//cyh add: �ڵ�ǰ��Ļָ��cursor�Ժ��λ�ö���ʾΪ�ո�' '
	char buf[1];
	buf[0] = 0;
	if ( CheckScreenValid() ) 
	{		
		memset(&screen.data[screen.l], ' ', SCREENSIZE - screen.l);
		screen.l = screen.cursor = SCREENSIZE;
		SendHstFrame(CMD_PointerPos, (char *)&buf, 1);
		SendHstFrame(CMD_StrDisplay, (char *)screen.data, sizeof(screen.data));
	}
}

void CHandset::ReverseBlock(INT8U startpos, INT8U endpos)
{
	if (startpos >= SCREENSIZE || endpos >= SCREENSIZE) return;
	INT8U buf[2];
	buf[0] = startpos;
	buf[1] = endpos;
	SendHstFrame(CMD_AntiScreen, (char *)buf, 2);
}

void CHandset::ReverseChar(INT8U pos)
{
	ReverseBlock(pos, pos);
}

void CHandset::ReverseLine(INT8U line)
{//cyh add: Ӧ���Ǹ���ǰ�м���Ӱ������?
	INT8U startpos;
    
	startpos = line * LINESIZE;
	ReverseBlock(startpos, startpos + (LINESIZE - 1));
}

void CHandset::ShowCursor(INT8U pos)
{
	ControlCursor(0x80, pos);
}

void CHandset::HideCursor(void)
{
	ControlCursor(0x00, 0);
}

bool CHandset::CheckScreenValid(void)
{
	if (screen.l > SCREENSIZE || screen.cursor > screen.l) return false;
	else 
		return true;
}
void CHandset::FilterChar(INT8U ch)
{
	INT8U cursor;
    
	if ((cursor = screen.cursor) >= SCREENSIZE) return;

	if (ch == '\r') 
	{
		cursor  = cursor - (cursor % LINESIZE);
	}
	else if (ch == '\n')
	{
		cursor += LINESIZE;
	}
	else
	{
		screen.data[cursor++] = ch;
	}

	if (cursor > SCREENSIZE) cursor = SCREENSIZE;

	screen.cursor = cursor;
	if (cursor > screen.l) screen.l = cursor;
}

void CHandset::ControlCursor(INT8U mode, INT8U pos)
{
	if (pos >= SCREENSIZE) pos = SCREENSIZE - 1;

	INT8U buf[2];
	buf[0] = mode;
	buf[1] = pos;
	SendHstFrame(CMD_CursorCtl, (char *)buf, 2);
}

/********************TextDply****************************************************/
void CHandset::UpdateDply(void)
{
    INT8U  i, linesize, numline;
    INT16U line, len;

    linesize = GetScreenLineSize();
    numline  = GetScreenNumLine();
    line     = curline;
    
    VTL_ClearScreen();

    if (maxline > 0) 
	{
        for (i = 0; i < numline; i++) 
		{
            if (line >= maxline) break;

            VTL_AbsMovCursor(i * linesize);

            if (line == maxline - 1) 
			{
                len = textlen - linepos[line];
			}
            else
			{
                len = linepos[line + 1] - linepos[line];
			}

            VTL_ScreenPrintMem(m_textptr + linepos[line], len);
            line++;
        } 
    } 
	else
	{
        VTL_ScreenPrintStr("�޼�¼");
    }

    OverWriteScreen();
}

void CHandset::InitTextDply(INT8U *ptr, INT16U len)
{
	memset(m_textptr,0x00,sizeof(m_textptr)); //cyh add:add

	memcpy(m_textptr, ptr, len);

    textlen = len;
	maxline = PlotLinePos(m_textptr, textlen, linepos, sizeof(linepos), GetScreenLineSize() );
 
	if ( maxline == 0xff )     return;

    curline = 0;
    UpdateDply();
}

bool CHandset::PageDownText(void)
{
    INT8U numline;
    
    if (maxline == 0) return FALSE;

    numline = GetScreenNumLine();

    if (curline + numline >= maxline)
	{
		return FALSE;  
	}
    else
	{
        curline += numline;
        UpdateDply();
        return TRUE;
    }
}

bool CHandset::PageUpText(void)
{
    INT8U numline;
    
    numline = GetScreenNumLine();
    if (maxline <= numline || curline < numline)
		return FALSE;  //cyh add: �����ǰ��Ļ������ȫ��ʾ�����Ѿ��޷����Ϸ���
    else 
	{//cyh add ���򣬵���Ҫ��ʾ������
        curline -= numline;
        UpdateDply();
        return TRUE;
    }
}

void CHandset::PageHomeText(void)
{
    curline = 0;
    UpdateDply();
}

void CHandset::PageEndText(void)
{
    INT8U numline;
    
    numline = GetScreenNumLine();
    if (maxline <= numline) curline = 0;
    else {
        curline = maxline % numline;
        if (curline == 0) curline = numline;
        curline = maxline - curline;
    }
    UpdateDply();
}

/*****************TextWin*******************************************************/
void CHandset::CreateTextWin(INT8U *textptr, INT16U textlen)
{
    textwin.textptr = textptr;
    textwin.textlen = textlen;
    InitTextDply(textptr, textlen);
}

INT16U CHandset::StoreTextWin(INT8U *storeptr)
{
    memcpy(storeptr, &textwin, sizeof(textwin));
    return sizeof(textwin);
}

void CHandset::ResumeTextWin(INT8U *storeptr)
{
    memcpy(&textwin, storeptr, sizeof(textwin));
    InitTextDply(textwin.textptr, textwin.textlen);
}

void CHandset::PageUpTextWin(void)
{
    if (!PageUpText()) PageEndText();
}

void CHandset::PageDownTextWin(void)
{
    if (!PageDownText()) PageHomeText();
}

/******************Window******************************************************/
void CHandset::CreateWin(WINDOWENTRY_STRUCT *entry)
{
    winattrib = entry->attrib;

    switch (winattrib)
	{
        case WIN_TEXT:
            CreateTextWin(entry->textptr, entry->textlen);
            break;
        case WIN_LMENU:
            CreateLMenuWin(entry->initrecord, entry->maxrecord, entry->readproc);
            break;
        case WIN_FMENU:
            CreateFMenuWin(entry->initrecord, entry->maxrecord, entry->readproc);
            break;
        default:
            return;
    }
}

INT16U CHandset::StoreWin(INT8U *storeptr)
{
    *storeptr++ = winattrib;
    switch (winattrib)
    {
        case WIN_TEXT:
            return StoreTextWin(storeptr);
        case WIN_LMENU:
            return StoreLMenuWin(storeptr);
        case WIN_FMENU:
            return StoreFMenuWin(storeptr);
        default:
            return 0;
    }
}

void CHandset::ResumeWin(INT8U *storeptr)
{
    winattrib = *storeptr++;    
    switch (winattrib)
    {
        case WIN_TEXT:
            ResumeTextWin(storeptr);
            break;
        case WIN_LMENU:
            ResumeLMenuWin(storeptr);
            break;
        case WIN_FMENU:
            ReasumeFMenuWin(storeptr);
            break;
        default:
            return;
    }
}

void CHandset::PageUpWin(void)
{
    switch (winattrib)
    {
        case WIN_TEXT:
            PageUpTextWin();            
            break;
        case WIN_LMENU:
            PageUpLMenuWin();
            break;
        case WIN_FMENU:
            PageUpFMenuWin();
            break;
        default:
            return;
    }
}

void CHandset::PageDownWin(void)
{
    switch (winattrib)
    {
        case WIN_TEXT:
            PageDownTextWin();
            break;
        case WIN_LMENU://sms
            PageDownLMenuWin();
            break;
        case WIN_FMENU:
            PageDownFMenuWin();
            break;
        default:
            return;
    }
}

INT16S CHandset::GetCurWinRecord(void)
{
    switch (winattrib)
    {
        case WIN_LMENU:
            return GetLMenuCurRecord();
        case WIN_FMENU:
            return GetFMenuCurRecord();
        default:
            return -1;
    }
}

/******************WinStack******************************************************/
void CHandset::InitWinStack()
{
    curstack = -1;
}

void CHandset::PopWinStack(void)	//���ٸý����ջ,�������ϲ�����ջ
{
    void (*Proc)(void);	
    if (curstack == -1 || curstack >= MAX_STACK) 
		return;

    Proc = winstack[curstack--].DestroyProc;

    if (Proc != 0)
		Proc();

    if (curstack != -1) 
	{
        Proc = winstack[curstack].ResumeProc;
        if (/*permitresume &&*/ Proc != 0) Proc();
    }
}

void CHandset::PushWinStack(WINSTACKENTRY_STRUCT *WinStackEntry)	//��ʼ���ö�ջ����,��������ʼ��
{
    void (*Proc)(void);

    if (curstack != -1) 
	{
        Proc = winstack[curstack].SuspendProc;
        
		if (Proc != 0) 
			Proc();
    }

    if (++curstack >= MAX_STACK)
	{
		--curstack;
		return;
	}
    winstack[curstack].InitProc    = WinStackEntry->InitProc;
    winstack[curstack].DestroyProc = WinStackEntry->DestroyProc;
    winstack[curstack].SuspendProc = WinStackEntry->SuspendProc;
    winstack[curstack].ResumeProc  = WinStackEntry->ResumeProc;
    winstack[curstack].HdlKeyProc  = WinStackEntry->HdlKeyProc;
    Proc = winstack[curstack].InitProc;
   
	if (Proc != 0) 
		Proc();
}

void CHandset::WinStackProc(void)	//����ý����µİ�����Ӧ
{
    void (*Proc)(void);

    if (curstack == -1)
		return;

    if (curstack >= MAX_STACK)
		return;

    Proc = winstack[curstack].HdlKeyProc;
    if (Proc != 0) Proc();
}

void CHandset::DestroyWinStack(void)
{//cyh add �������еĽ���
    if (ReserveLowestStack()) PopWinStack();
}

void CHandset::ActivateLowestWinStack(void)
{  //cyh add �ص���ײ�Ľ���
    void (*Proc)(void);

    if (ReserveLowestStack()) {
        Proc = winstack[0].ResumeProc;
        if (Proc != 0) Proc();
    }
}

INT8U *CHandset::GetWinStackEnviroment(void)
{
	INT8U *pEnv = NULL;

    if (curstack == -1 || curstack >= MAX_STACK) return 0;
    pEnv = winstack[curstack].enviroment;

	return pEnv;
}

bool CHandset::ReserveLowestStack(void)
{//cyh add ��������ײ�Ľ��棬�����ȫ������
    void (*Proc)(void);

    if (curstack == -1) return false;
    for (;;) {
        if (curstack == 0) return true;
        Proc = winstack[curstack--].DestroyProc;
        if (Proc != 0) Proc();
    }
}

INT8S CHandset::GetCurstack(void)
{
	return curstack;
}

/**************AuxFunc**********************************************************/
bool CHandset::NeedRoundLine(INT8U *ptr, INT16U len)
{//cyh add:�ƺ���Ϊ��ʹ��Ļ������ʾ һ���������ַ�������ɴ�ͽ�����ַ��ƶ�����һ������ʾ�������UnicodeӦ�þͲ��������������˰�
  //cyh add: Ϊ��ʹ��Ļ������ɵ���ʾ���ģ���������ֽڶ�����0xa1�����Ǻ���
    bool isgbcode;

    for (isgbcode = FALSE; len > 0; ptr++, len--) 
	{
        if (!isgbcode)
		{
            if (*ptr >= 0xa1) isgbcode = TRUE;
        } 
		else 
		{
            if (*ptr >= 0xa1) isgbcode = FALSE;
        }
    }
    return isgbcode;
}

/************************************************************************
* ����˵��:																*
*		textptr:	��������											*
*		textlen:	���ݳ���											*
*		linpos:		��λ��												*
*		maxline:	��������(50)										*
*		linesize:	���е����ݴ�С(12)									*
*����ֵ:																*
*		��������,ͬʱ�������ݵ���λ��									*
/************************************************************************/
INT16U CHandset::PlotLinePos(INT8U *textptr, INT16U textlen, INT16U *linepos, INT16U maxline, INT16U linesize)
{
	//posΪ����ָ���λ��,lenΪ��ǰ��ָ��λ��,lineΪ����,lineposΪ��ǰ��λ��

//cyh add: ���ܣ��ú��� ����ǰҪ��ʾ���ַ����� �ֽ�Ϊn�У�����ÿ�е���ʼ���� ����linepos����
//cyh add  pos:����ָ�뼴��ǰָ��textptr�е���һ�����ݣ���Χ[0,textlen-1]
//cyh add  len: ��ǰ�е�ָ��λ�ã���Χ[0,11]
//cyh add  linepos:�����ÿ����Ա��������Ӧ�е� ��ʼ���� ��ָ�롣linepos[1]��� �ڵ�һ����ʾ����ʼ���ݵ�λ�ã���Χ[0,textlen-1]
//cyh add  line: textptr�е����ݷּ�����ʾ
	INT16U pos, len, line;
    pos = line = len = linepos[0] = 0;
    if (textlen == 0) return 0;

    for (;;) 
	{
        if (*textptr++ == '\n')
		{//���ת��
            if (++line > maxline) return 0xffff;
            len = 0;
            linepos[line] = ++pos;	//��λ����Ϊ��ǰ�е�λ��
        }
		else 
		{
            pos++;
            if (++len == linesize)
			{	//��ǰָ��λ�ôﵽ���е����λ��
                if (NeedRoundLine(textptr - linesize, linesize))  //cyh add:�ƺ���Ϊ��ʹ��Ļ������ʾ һ���������ַ�������ɴ�ͽ�����ַ��ƶ�����һ������ʾ�������UnicodeӦ�þͲ��������������˰�
				{
                    pos--;
                    textptr--;
                }

                if (++line > maxline) return 0xffff;

                len = 0;
                linepos[line] = pos;	//��λ����Ϊ��ǰ�е�λ��
            }
        }

        if (pos > textlen - 1) 
		{		//ָ�볬�����ݳ���
            if (len == 0) line--;		//�������������һ�У���������һ
            break;
        }
    }

    return (line + 1);
}

INT16U CHandset::InsertEditBuf(INT8U *ptr, INT16U len, INT16U pos, INT16U ch)
{
    INT16U i;
    INT8U  temp, *tempptr, ret;

    if (pos > len) return len;
    if (ch >= 0xA100)
        ret = 2;
    else
        ret = 1;
    for (;;) {
        tempptr = ptr + len;
        for (i = 1; i <= len - pos; i++) {
            temp       = *(tempptr - 1);
            *tempptr-- = temp;
        }
        if (ret == 2)
            *tempptr = ch >> 8;
        else
            *tempptr = ch;
        len++;
        pos++;
        if (--ret == 0) return len;
    }
}

INT8U CHandset::DelEditBuf(INT8U *ptr, INT16U len, INT16U pos)
{
    INT16U j;
    INT8U  i, temp, *tempptr, ret;

    ret = 0;
    if (pos > len || pos == 0) return 0;

    if (ptr[pos - 1] >= 0xA1)  ret = 2;
    else  ret = 1;
       

    if (pos < ret) return 0;

    for (i = 1; i <= ret; i++) 
	{
        tempptr = ptr + pos;
        for (j = 1; j <= (len - pos); j++) 
		{
            temp           = *tempptr;
            *(tempptr - 1) = temp;
            tempptr++;
        }
        len--;
        pos--;
    }

    return ret;
}

void CHandset::WriteWordByPointer(INT8U *ptr, INT16U wdata)
{
    *ptr++ = wdata >> 8;
    *ptr   = wdata;
}

INT16U CHandset::SearchDigitalString(INT8U *ptr, INT16U maxlen, INT8U flagchar, INT8U numflag)
{
    BOOLEAN find;
    INT16U  result;
    INT8U *bptr, *eptr;

    find = FALSE;
    for(;;) {
        if (maxlen == 0) {
            find = FALSE;
            break;
        }
        if (*ptr == flagchar) {
            numflag--;
            if (numflag == 0) {
                eptr = ptr;
                break;
            }
        }
        if (*ptr >= '0' && *ptr <= '9') {
            if (!find) {
                find = TRUE;
                bptr = ptr;
            }
        } else {
            find = FALSE;
        }
        ptr++;
        maxlen--;
    }

    if (find) {
        result = 0;
        for(;;) {
            result = result * 10 + CharToHex(*bptr++);
            if (bptr == eptr) break;
        }
    } else {
        result = 0xffff;
    }
    return result;
}

INT8U CHandset::CharToHex(INT8U schar)
{
    if (schar >= '0' && schar <= '9') return (schar - '0');
    else if (schar >= 'A' && schar <= 'F') return (schar - 'A' + 0x0A);
    else if (schar >= 'a' && schar <= 'f') return (schar - 'a' + 0x0A);
    else return 0;
}

INT16U CHandset::ReadWordByPointer(INT8U *ptr)
{
    INT16U temp;

    temp  = *ptr++;
    temp = temp << 8;
    temp |= *ptr;
    return temp;
}

//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: У���Ƿ���ȷ
bool CHandset::check_crc(const byte *buf, const int len)
{   
	byte check_sum = get_crc( buf+1, len-1 );
	return (buf[0]==check_sum);
}

//--------------------------------------------------------------------------------------------------------------------------
// �õ�У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: ���ݵ�У���
byte CHandset::get_crc(const byte *buf, const int len)
{   
	byte check_sum = 0;
	for(int i=0; i<len; i++) {
		check_sum += *(buf+i);
	}
    return check_sum;
}


