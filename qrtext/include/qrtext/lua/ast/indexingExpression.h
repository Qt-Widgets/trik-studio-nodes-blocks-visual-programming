/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT IndexingExpression : public Expression
{
public:
	IndexingExpression(QSharedPointer<Expression> const &table, QSharedPointer<Expression> const &indexer)
		: mTable(table), mIndexer(indexer)
	{
	}

	QSharedPointer<Expression> const &table() const {
		return mTable;
	}

	QSharedPointer<Expression> const &indexer() const {
		return mIndexer;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mTable, mIndexer};
	}

private:
	void accept(core::AstVisitorInterface &visitor) const override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
	}

	QSharedPointer<Expression> mTable;
	QSharedPointer<Expression> mIndexer;
};

}
}
}
