/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef CURRENTPROJECTFILTER_H
#define CURRENTPROJECTFILTER_H

#include <locator/basefilefilter.h>

#include <QFutureInterface>

namespace ProjectExplorer {

class ProjectExplorerPlugin;
class Project;

namespace Internal {

class CurrentProjectFilter : public Locator::BaseFileFilter
{
    Q_OBJECT

public:
    CurrentProjectFilter(ProjectExplorerPlugin *pe);
    QString displayName() const { return tr("Files in Current Project"); }
    QString id() const { return QLatin1String("Files in current project"); }
    Locator::ILocatorFilter::Priority priority() const { return Locator::ILocatorFilter::Low; }
    void refresh(QFutureInterface<void> &future);

protected:
    void updateFiles();

private slots:
    void currentProjectChanged(ProjectExplorer::Project *project);
    void markFilesAsOutOfDate();

private:

    ProjectExplorerPlugin *m_projectExplorer;
    Project *m_project;
    bool m_filesUpToDate;
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // CURRENTPROJECTFILTER_H
