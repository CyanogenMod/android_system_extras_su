#define LOG_TAG "su"

#include <binder/AppOpsManager.h>
#include <utils/Log.h>

using namespace android;

#define APPOPS_OP_SU 68 // This must match the number in AppOpsManager

extern "C" {

int appops_start_op_su(int uid, const char *pkgName) {
    ALOGD("Checking whether app [uid:%d, pkgName: %s] is allowed to be root", uid, pkgName);
    AppOpsManager *ops = new AppOpsManager();

    int mode = ops->startOp(APPOPS_OP_SU, uid, String16(pkgName));

    switch (mode) {
        case AppOpsManager::MODE_ALLOWED:
          ALOGD("Privilege elevation allowed by appops");
          return 0;
        default:
          ALOGD("Privilege elevation denied by appops");
          return 1;
    }

    free(ops);
}

void appops_finish_op_su(int uid, const char *pkgName) {
    ALOGD("Finishing su operation for app [uid:%d, pkgName: %s]", uid, pkgName);
    AppOpsManager *ops = new AppOpsManager();
    ops->finishOp(APPOPS_OP_SU, uid, String16(pkgName));
    free(ops);
}

}
