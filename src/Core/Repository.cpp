
#include "Core/Git_p.h"
#include "Core/Index.h"
#include "Core/Repository.h"

#include "git2/branch.h"

namespace Git
{

	RepositoryPrivate::RepositoryPrivate( git_repository* repo )
		: mRepo( repo )
		, mIndex( NULL )
	{
	}

	RepositoryPrivate::~RepositoryPrivate()
	{
		Q_ASSERT( mRepo );
		Q_ASSERT( !mIndex );

		git_repository_free( mRepo );
	}

	void RepositoryPrivate::setError( int rc )
	{
		Q_UNUSED( rc );
	//	QString errText = QString::fromUtf8( git_lasterror() );
	//	qDebug( "%s", qPrintable( errText ) );
	//	MainWindow::self().addError( errText );
	}

	Repository::Repository( RepositoryPrivate* _d )
		: d( _d )
	{
	}

	Repository::Repository( const Repository& o )
		: d( o.d )
	{
	}

	Repository::Repository()
	{
	}

	Repository::~Repository()
	{
	}

	Repository& Repository::operator=( const Repository& o )
	{
		d = o.d;
		return * this;
	}

	bool Repository::isValid() const
	{
		return d;
	}

	Repository Repository::create( const QString& path, bool bare )
	{
		git_repository* repo = NULL;

		int rc = git_repository_init( &repo, path.toLatin1().constData(), bare );
		if( rc < GIT_SUCCESS )
		{
		//	d->setError( rc );
			return Repository();
		}

		return Repository( new RepositoryPrivate( repo ) );
	}

	Repository Repository::open( const QString& path )
	{
		git_repository* repo = NULL;

		int rc = git_repository_open( &repo, path.toLatin1().constData() );

		if( rc < GIT_SUCCESS )
		{
		//	d->setError( rc );
			return Repository();
		}

		return Repository( new RepositoryPrivate( repo ) );
	}

	bool Repository::isBare() const
	{
		Q_ASSERT( d );
		return git_repository_is_bare( d->mRepo );
	}

	Index Repository::index()
	{
		Q_ASSERT( d );

		if( isBare() )
		{
			return Index();
		}

		if( !d->mIndex )
		{
			git_index* index = NULL;

			int rc = git_repository_index( &index, d->mRepo );

			if( rc < GIT_SUCCESS )
			{
				d->setError( rc );
				return Index();
			}

			d->mIndex = new IndexPrivate( d, index );
		}

		return Index( d->mIndex );
	}

	QStringList slFromStrArray( git_strarray* arry )
	{

		QStringList sl;
		for( unsigned int i = 0; i < arry->count; i++ )
			sl << QString::fromLatin1( arry->strings[ i ] );

		git_strarray_free( arry );
		return sl;
	}

	QStringList Repository::allReferences()
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_reference_listall( &arr, d->mRepo, GIT_REF_LISTALL );
		if( rc < GIT_SUCCESS )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}

	QStringList Repository::allBranches()
	{
		return branches( true, true );
	}

	QStringList Repository::branches( bool local, bool remote )
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_branch_list( &arr, d->mRepo,
								  ( local ? GIT_BRANCH_LOCAL : 0 ) |
								  ( remote ? GIT_BRANCH_REMOTE : 0 ) );
		if( rc < GIT_SUCCESS )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}

	QStringList Repository::allTags()
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_tag_list( &arr, d->mRepo );
		if( rc < GIT_SUCCESS )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}


	int status_callback( const char* name, unsigned int status, void* )
	{
		qDebug( "%s - %s",
				qPrintable( QString::number( status, 2 ) ),
				qPrintable( name ) );
		return GIT_SUCCESS;
	}

	void Repository::test()
	{
		git_status_foreach( d->mRepo, &status_callback, 0 );
	}

	static int statusHashCB( const char* fn, unsigned int status, void* rawSH )
	{
		StatusHash* sh = (StatusHash*) rawSH;
		sh->insert( QString::fromUtf8( fn ), status );
		return GIT_SUCCESS;
	}

	StatusHash Repository::statusHash()
	{
		StatusHash sh;

		if( d )
		{
		//	git_status_foreach( d->mRepo, &statusHashCB, (void*) &sh );
			git_status_options opt;
			memset( &opt, 0, sizeof( opt ) );
			opt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED
					  | GIT_STATUS_OPT_INCLUDE_IGNORED
					  | GIT_STATUS_OPT_INCLUDE_UNMODIFIED
					  | GIT_STATUS_OPT_EXCLUDE_SUBMODULES
					  | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
			git_status_foreach_ext( d->mRepo, &opt, &statusHashCB, (void*) &sh );
		}

		return sh;
	}

	QString Repository::basePath() const
	{
		return QString::fromUtf8(( git_repository_workdir( d->mRepo ) ) );
	}

}
