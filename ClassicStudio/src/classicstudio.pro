include(settings/settings.pro)
TEMPLATE = subdirs
CONFIG  += c++11

SUBDIRS += commonui \
           communication \
           audio \
           behavior \
           datacenter \
           hts \
           robotcfg \
           robotsearch \
           servoeditor \
           behavioredit \
           fbx \
           robotview \
           mainframe \
           updater \
           utils

robotsearch.depends = robotcfg

robotview.depends = fbx \
                    robotcfg \
                    datacenter \
                    utils

servoeditor.depends = robotcfg \
                      datacenter \
                      commonui \
                      utils

behavioredit.depends = robotcfg \
                       audio \
                       commonui \
                       datacenter \
                       utils

mainframe.depends = robotcfg \
                    behavior \
                    communication \
                    servoeditor \
                    behavioredit \
                    robotview \
                    updater \
                    commonui \
                    datacenter \
                    utils

datacenter.depends = robotcfg \
                     behavior \
                     hts \
                     communication \
                     robotsearch \
                     utils

communication.depends = utils

HEADERS += $$PWD/common/commondefine.h

TRANSLATIONS = yansheestudio.ts
