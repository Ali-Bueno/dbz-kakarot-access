/* Byte-exact layouts for the UI classes relevant to the two Lua dead ends
   (battle-pause selection + field-menu section). Extracted from the UE4SS
   CXXHeaderDump (AT.hpp). Pointers->void*, aggregates->raw bytes, all gaps
   padded so every offset is exact. GAPS AFTER the last reflected member are
   where the non-UPROPERTY C++ members (hidden selection indices) live -> the
   Ghidra decompilation of the cursor-move methods should write into them. */
#include <stdint.h>

// UAT_UIXCmnPause : UCFUIUserWidget   size=0x500
struct UAT_UIXCmnPause {
    uint8_t _pad_0000[0x398];
    void* AllNut;                           // 0x0398  <= class UCanvasPanel*
    void* LangPauseImg;                     // 0x03a0  <= class UImage*
    uint8_t ListBarArray[0x10];             // 0x03a8  <= TArray<class UAT_UIGameoverBar*>
    void* UIStartTips;                      // 0x03b8  <= class UAT_UIStartTips*
    void* UITips;                           // 0x03c0  <= class UAT_UITips*
    void* UIXCmnHeader;                     // 0x03c8  <= class UAT_UIXCmnHeader*
    void* InAnim;                           // 0x03d0  <= class UWidgetAnimation*
    void* LoopAnim;                         // 0x03d8  <= class UWidgetAnimation*
    void* OutAnim;                          // 0x03e0  <= class UWidgetAnimation*
    uint8_t Loader_Map[0x50];               // 0x03e8  <= TMap<class FName, class UATDataAssetUiAsyncLoad*>
    uint8_t _tailgap_0438[0xc8];  // 0x0438..0x500  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x500

// UAT_UIGameoverBar : UCFUIUserWidget   size=0x3c8
struct UAT_UIGameoverBar {
    uint8_t _pad_0000[0x398];
    uint8_t DelegateGameoverBarEndAnimation[0x10];// 0x0398  <= FAT_UIGameoverBarDelegateGameoverBarEndAnimation
    void* RootPanel;                        // 0x03a8  <= class UCanvasPanel*
    void* PnlCursEff;                       // 0x03b0  <= class UBorder*
    void* TxtList;                          // 0x03b8  <= class UCFUIXcmnMultiLineText*
    void* AnimEnter;                        // 0x03c0  <= class UWidgetAnimation*
};  // 0x3c8

// UAT_UIGameover : UCFUIUserWidget   size=0x460
struct UAT_UIGameover {
    uint8_t _pad_0000[0x398];
    void* RootPanel;                        // 0x0398  <= class UCanvasPanel*
    void* TitleImage;                       // 0x03a0  <= class UImage*
    void* AppearAnimation;                  // 0x03a8  <= class UWidgetAnimation*
    void* DisappearAnimation;               // 0x03b0  <= class UWidgetAnimation*
    uint8_t SelectionWidgetArray[0x10];     // 0x03b8  <= TArray<class UAT_UIGameoverBar*>
    float ShowGameOverTime;                 // 0x03c8  <= float
    uint8_t _pad_03cc[0x4];
    uint8_t DelegateGameoverEnd[0x10];      // 0x03d0  <= FAT_UIGameoverDelegateGameoverEnd
    int32_t CurrentSelectIndex;             // 0x03e0  <= int32
    uint8_t _tailgap_03e4[0x7c];  // 0x03e4..0x460  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x460

// UAT_UIXCmnHeader : UCFUIUserWidget   size=0x510
struct UAT_UIXCmnHeader {
    uint8_t _pad_0000[0x398];
    void* Canvas_AllNut;                    // 0x0398  <= class UCanvasPanel*
    void* Canvas_Header;                    // 0x03a0  <= class UCanvasPanel*
    void* Canvas_IconHeader;                // 0x03a8  <= class UCanvasPanel*
    void* Image_HeaderFont;                 // 0x03b0  <= class UImage*
    void* Image_IconFont;                   // 0x03b8  <= class UImage*
    void* Image_Icon;                       // 0x03c0  <= class UImage*
    void* Image_IconCaption;                // 0x03c8  <= class UImage*
    void* Anim_In;                          // 0x03d0  <= class UWidgetAnimation*
    void* Anim_Loop;                        // 0x03d8  <= class UWidgetAnimation*
    void* Anim_Out;                         // 0x03e0  <= class UWidgetAnimation*
    void* Anim_IconIn;                      // 0x03e8  <= class UWidgetAnimation*
    void* Anim_IconLoop;                    // 0x03f0  <= class UWidgetAnimation*
    void* Anim_IconOut;                     // 0x03f8  <= class UWidgetAnimation*
    uint8_t _tailgap_0400[0x110];  // 0x0400..0x510  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x510

// UAT_UIStartTop : UCFUIUserWidget   size=0x600
struct UAT_UIStartTop {
    uint8_t _pad_0000[0x398];
    float CursorRepeatWait;                 // 0x0398  <= float
    uint8_t _pad_039c[0x4];
    void* All_Nut;                          // 0x03a0  <= class UCanvasPanel*
    void* Dmy_Char;                         // 0x03a8  <= class UImage*
    void* Ins_Char_Eff00;                   // 0x03b0  <= class UImage*
    void* Ins_Char_Shdw;                    // 0x03b8  <= class UImage*
    void* Dmy_Char_Fad;                     // 0x03c0  <= class UImage*
    void* Xmenu_Bg;                         // 0x03c8  <= class UAT_UIXcmnBg*
    void* Xmenu_Cloud_Btm;                  // 0x03d0  <= class UAT_UIStartTopCloud*
    void* Shop_Cmn_Money;                   // 0x03d8  <= class UAT_UIShopCmnMoney*
    void* Start_Top_State;                  // 0x03e0  <= class UAT_UIStartTopState*
    void* Txt_Open_CompZ;                   // 0x03e8  <= class UCFUIXcmnMultiLineText*
    void* Start_Top_CompZ01;                // 0x03f0  <= class UCFUIUserWidget*
    void* Img_Icon_CompZ;                   // 0x03f8  <= class UImage*
    void* Ins_Sirc_CompZ00_01;              // 0x0400  <= class UImage*
    void* Pnl_DLC_Info;                     // 0x0408  <= class UCanvasPanel*
    void* Txt_DLC_Info;                     // 0x0410  <= class UCFUIXcmnMultiLineText*
    uint8_t _pad_0418[0x10];
    uint8_t UIStartTopList[0x10];           // 0x0428  <= TArray<class UAT_UIStartTopList*>
    uint8_t UIStartTopList04_Sub[0x10];     // 0x0438  <= TArray<class UAT_UIStartTopList*>
    uint8_t UIStartTopList06_Sub[0x10];     // 0x0448  <= TArray<class UAT_UIStartTopList*>
    uint8_t UIStartTopAllList[0x10];        // 0x0458  <= TArray<class UAT_UIStartTopList*>
    uint8_t UIStartTopListZ[0x10];          // 0x0468  <= TArray<class UAT_UIStartTopListZ*>
    void* Anim_In;                          // 0x0478  <= class UWidgetAnimation*
    void* Anim_Out;                         // 0x0480  <= class UWidgetAnimation*
    void* Anim_Enter_Commu;                 // 0x0488  <= class UWidgetAnimation*
    void* Anim_Close_Commu;                 // 0x0490  <= class UWidgetAnimation*
    void* Anim_Out_Commu;                   // 0x0498  <= class UWidgetAnimation*
    void* Anim_In_Commu;                    // 0x04a0  <= class UWidgetAnimation*
    void* Anim_Enter_System;                // 0x04a8  <= class UWidgetAnimation*
    void* Anim_Close_System;                // 0x04b0  <= class UWidgetAnimation*
    void* Anim_Out_System;                  // 0x04b8  <= class UWidgetAnimation*
    void* Anim_In_System;                   // 0x04c0  <= class UWidgetAnimation*
    void* Anim_OpenCompZLoop;               // 0x04c8  <= class UWidgetAnimation*
    void* UIXCmnHeader;                     // 0x04d0  <= class UAT_UIXCmnHeader*
    uint8_t _tailgap_04d8[0x128];  // 0x04d8..0x600  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x600

// UAT_UIStartTopList : UCFUIUserWidget   size=0x4c0
struct UAT_UIStartTopList {
    uint8_t _pad_0000[0x398];
    void* Canvas_AllNut;                    // 0x0398  <= class UCanvasPanel*
    void* Image_Deco00;                     // 0x03a0  <= class UImage*
    void* Image_Deco01;                     // 0x03a8  <= class UImage*
    void* Image_Deco02;                     // 0x03b0  <= class UImage*
    void* Image_Deco03;                     // 0x03b8  <= class UImage*
    void* Image_Deco04;                     // 0x03c0  <= class UImage*
    void* Image_CursTop;                    // 0x03c8  <= class UImage*
    void* Image_CursBottom;                 // 0x03d0  <= class UImage*
    void* Image_FontOn;                     // 0x03d8  <= class UImage*
    void* Image_Font;                       // 0x03e0  <= class UImage*
    void* Anim_In;                          // 0x03e8  <= class UWidgetAnimation*
    void* Anim_Loop;                        // 0x03f0  <= class UWidgetAnimation*
    void* Anim_Out;                         // 0x03f8  <= class UWidgetAnimation*
    uint8_t _tailgap_0400[0xc0];  // 0x0400..0x4c0  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x4c0

// UAT_UIStartTopListZ : UCFUIUserWidget   size=0x3b0
struct UAT_UIStartTopListZ {
    uint8_t _pad_0000[0x398];
    void* Image_Icon;                       // 0x0398  <= class UImage*
    void* Image_UnderScore;                 // 0x03a0  <= class UImage*
    void* TextBox_Number;                   // 0x03a8  <= class UCFUIXcmnMultiLineText*
};  // 0x3b0

// UAT_UIMenuListBase00 : UCFUIUserWidget   size=0x4a0
struct UAT_UIMenuListBase00 {
    uint8_t _pad_0000[0x398];
    uint8_t ListPlateNameCtn[0x10];         // 0x0398  <= TArray<FName>
    uint8_t ListPlateCtn[0x10];             // 0x03a8  <= TArray<class UAT_UIList00Choice*>
    void* CanvasAllNut;                     // 0x03b8  <= class UCanvasPanel*
    void* ImgTitleBase;                     // 0x03c0  <= class UImage*
    void* TxtTitle;                         // 0x03c8  <= class UCFUIXcmnMultiLineText*
    void* TxtNum;                           // 0x03d0  <= class UCFUIXcmnMultiLineText*
    void* ScrollBar;                        // 0x03d8  <= class UAT_UIScrollBar*
    void* Image_Dmy_Space;                  // 0x03e0  <= class UImage*
    void* InAnim;                           // 0x03e8  <= class UWidgetAnimation*
    void* OutAnim;                          // 0x03f0  <= class UWidgetAnimation*
    uint8_t SelectCheakList[0x50];          // 0x03f8  <= TMap<int32, int32>
    void* AllCurs;                          // 0x0448  <= class UHorizontalBox*
    uint8_t _tailgap_0450[0x50];  // 0x0450..0x4a0  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x4a0

// UAT_UIMenuListBase01 : UCFUIUserWidget   size=0x410
struct UAT_UIMenuListBase01 {
    uint8_t _pad_0000[0x398];
    uint8_t ListPlateCtn[0x10];             // 0x0398  <= TArray<class UAT_UIList01Choice*>
    void* CanvasAllNut;                     // 0x03a8  <= class UCanvasPanel*
    void* TxtTitle;                         // 0x03b0  <= class UCFUIXcmnMultiLineText*
    void* TxtNum;                           // 0x03b8  <= class UCFUIXcmnMultiLineText*
    void* ScrollBar;                        // 0x03c0  <= class UAT_UIScrollBar*
    void* InAnim;                           // 0x03c8  <= class UWidgetAnimation*
    void* OutAnim;                          // 0x03d0  <= class UWidgetAnimation*
    void* InFadeAnim;                       // 0x03d8  <= class UWidgetAnimation*
    void* OutFadeAnim;                      // 0x03e0  <= class UWidgetAnimation*
    void* AllCurs;                          // 0x03e8  <= class UBorder*
    uint8_t _tailgap_03f0[0x20];  // 0x03f0..0x410  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x410

// UAT_UIMenuListBase03 : UCFUIUserWidget   size=0x3f0
struct UAT_UIMenuListBase03 {
    uint8_t _pad_0000[0x398];
    uint8_t ListPlateCtn[0x10];             // 0x0398  <= TArray<class UAT_UIList03Choice*>
    void* CanvasAllNut;                     // 0x03a8  <= class UCanvasPanel*
    void* TxtTitle;                         // 0x03b0  <= class UCFUIXcmnMultiLineText*
    void* ScrollBar;                        // 0x03b8  <= class UAT_UIScrollBar*
    void* InAnim;                           // 0x03c0  <= class UWidgetAnimation*
    void* OutAnim;                          // 0x03c8  <= class UWidgetAnimation*
    uint8_t _tailgap_03d0[0x20];  // 0x03d0..0x3f0  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x3f0

// UAT_UITips : UCFUIUserWidget   size=0x508
struct UAT_UITips {
    uint8_t _pad_0000[0x398];
    float InputWaitTime;                    // 0x0398  <= float
    float InputWaitTimeFromMenu;            // 0x039c  <= float
    float NextInputWaitTime;                // 0x03a0  <= float
    uint8_t _pad_03a4[0x4];
    void* Canvas_AllNut;                    // 0x03a8  <= class UCanvasPanel*
    void* Widget_Fade;                      // 0x03b0  <= class UCFUIUserWidget*
    void* UIXcmnArrowL;                     // 0x03b8  <= class UAT_UIXcmnArrow*
    void* UIXcmnArrowR;                     // 0x03c0  <= class UAT_UIXcmnArrow*
    uint8_t UITipsWin_List[0x10];           // 0x03c8  <= TArray<class UAT_UITipsWin*>
    void* UIImageFull;                      // 0x03d8  <= class UImage*
    void* UICloudBtm;                       // 0x03e0  <= class UAT_UIStartTopCloud*
    void* Anim_In;                          // 0x03e8  <= class UWidgetAnimation*
    void* Anim_Out;                         // 0x03f0  <= class UWidgetAnimation*
    void* Anim_Change;                      // 0x03f8  <= class UWidgetAnimation*
    void* UIXCmnHeader;                     // 0x0400  <= class UAT_UIXCmnHeader*
    void* Table_TipsParam;                  // 0x0408  <= class UDataTable*
    uint8_t _pad_0410[0x70];
    uint8_t NextTipsIds[0x10];              // 0x0480  <= TArray<FName>
    uint8_t _tailgap_0490[0x78];  // 0x0490..0x508  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x508

// UMenuManager : UActorComponent   size=0x220
struct UMenuManager {
    uint8_t _pad_0000[0x108];
    void* m_xRootMenu;                      // 0x0108  <= class URootMenu*
    void* m_xDragonBallMenu;                // 0x0110  <= class UDragonBallMenu*
    void* m_xSceneSummonsShenlong;          // 0x0118  <= class USceneSummonsShenlong*
    void* m_xPartyMenu;                     // 0x0120  <= class UPartyMenu*
    void* m_xCharacterMenu;                 // 0x0128  <= class UCharacterMenu*
    void* m_xStatusMenu;                    // 0x0130  <= class UStatusMenu*
    void* m_xItemInventoryMenu;             // 0x0138  <= class UItemInventoryMenu*
    void* m_xQuestMenu;                     // 0x0140  <= class UQuestMenu*
    void* m_xStartSkillTreeMenu;            // 0x0148  <= class UStartSkillTreeMenu*
    void* m_xStartSkillCustomize;           // 0x0150  <= class UStartSkillCustomize*
    void* m_xSoulEmblemMenu;                // 0x0158  <= class USoulEmblemMenu*
    void* m_xCommunityBoard;                // 0x0160  <= class UCommunityBoardMenu*
    void* m_xTipsMenu;                      // 0x0168  <= class UTipsMenu*
    void* m_xSaveMenu;                      // 0x0170  <= class USaveMenu*
    void* m_xLoadMenu;                      // 0x0178  <= class ULoadMenu*
    void* m_xOptionMenu;                    // 0x0180  <= class UOptionMenu*
    void* m_xCompZMenu;                     // 0x0188  <= class UCompZMenu*
    void* m_xDlcMenu;                       // 0x0190  <= class UDlcMenu*
    void* m_xScreenCapture;                 // 0x0198  <= class UAT_UIScreenCapture*
    uint8_t m_xMenuList[0x10];              // 0x01a0  <= TArray<class UBaseMenu*>
    uint8_t _tailgap_01b0[0x70];  // 0x01b0..0x220  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x220

// URootMenu : UBaseMenu   size=0x160
struct URootMenu {
    uint8_t _pad_0000[0x120];
    void* m_UIStartTop;                     // 0x0120  <= class UAT_UIStartTop*
    uint8_t _tailgap_0128[0x38];  // 0x0128..0x160  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x160

// UBaseMenu : UBaseObject   size=0x120
struct UBaseMenu {
    uint8_t _pad_0000[0xa0];
    void* m_pManager;                       // 0x00a0  <= class UMenuManager*
    uint8_t _tailgap_00a8[0x78];  // 0x00a8..0x120  <=== UNLISTED C++ MEMBERS (hidden index candidates)
};  // 0x120

