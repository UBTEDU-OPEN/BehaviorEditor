#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H

namespace UBTYanshee {
    enum MirrorType {
        eMirrorTypeRightArmToLeft,
        eMirrorTypeLeftArmToRight,
        eMirrorTypeRightLegToLeft,
        eMirrorTypeLeftLegToRight
    };

    enum ReversalType {
        eReversalTypeFull,
        eReversalTypeHead,
        eReversalTypeArm,
        eReversalTypeLeg
    };

    enum LayerType {
        EM_LAYER_TYPE_UNKNOW = 0,
        EM_LAYER_TYPE_ACTION,           /**< 动作层 */
        EM_LAYER_TYPE_MUSIC,            /**< 音乐层 */
        EM_LAYER_TYPE_LED               /**< led层 */
    };

    enum RunMode {
        EM_RUN_MODE_UNKNOW = 0,
        EM_RUN_MODE_UNIFORM_SPEED,      /**< 匀速 */
        EM_RUN_MODE_CURVE_SPEED         /**< 曲线 */
    };

}

#endif // COMMONDEFINE_H
