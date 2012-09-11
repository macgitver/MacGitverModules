/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GIT_REVISION_WALKER_H
#define GIT_REVISION_WALKER_H

#include "Git.h"

namespace Git
{

	class RevisionWalkerPrivate;

	class ObjectId;
	class Reference;

    /**
     * @brief The RevisionWalker class provides functionality to access a Git repository's history.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API RevisionWalker
	{
	public:
		RevisionWalker();
		RevisionWalker( RevisionWalkerPrivate* _d );
		~RevisionWalker();
		RevisionWalker& operator=( const RevisionWalker& other );

	public:
		bool isValid() const;

		void reset();
		void push( const ObjectId& id );
		void push( const Reference& ref );
		void pushRef( const QString& name );
		void pushHead();

		bool next( ObjectId& oidNext );
		QVector< ObjectId > all();

		void setSorting( bool topological, bool timed );

	private:
		GitPtr< RevisionWalkerPrivate > d;
	};

    /**@}*/
}

#endif
