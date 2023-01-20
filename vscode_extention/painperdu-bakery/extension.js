const vscode = require('vscode');
const bindings = require('./generated/ExtentionBindings')

const tokenTypeStrings = [
	'variable',
	'function',
	'comment',
	'string',
	'number',
	'operator'
];
const legend = new vscode.SemanticTokensLegend(tokenTypeStrings);


class DocumentSemanticTokensProvider {
	async provideDocumentSemanticTokens(document) {
		const allTokens = bindings.get_tokens(document.getText());
		const builder = new vscode.SemanticTokensBuilder(legend);
		for (var i = 0; i < allTokens.size(); ++i) {
			let token = allTokens.get(i);
			builder.push(token.get_line() - 1, token.get_start_column() - 1, token.get_length(), token.get_type_index());
		}
		return builder.build();
	}
}

class GoCompletionItemProviderLabels {
	provideCompletionItems(document) {
		const labels = bindings.get_defined_labels(document.getText());

		let result = [];
		for (var i = 0; i < labels.size(); ++i) {
			let label = labels.get(i);
			let completionItem = new vscode.CompletionItem();
			completionItem.label = label;
			completionItem.kind = vscode.CompletionItemKind.Function;
			result.push(completionItem);
		}
		return result;
	}
}

class GoCompletionItemProviderReference {
	provideCompletionItems(document) {
		const refs = bindings.get_defined_references(document.getText());

		let result = [];
		for (var i = 0; i < refs.size(); ++i) {
			let ref = refs.get(i);
			let completionItem = new vscode.CompletionItem();
			completionItem.label = ref;
			completionItem.kind = vscode.CompletionItemKind.Variable;
			result.push(completionItem);
		}
		return result;
	}
}

/**
 * @param {vscode.ExtensionContext} context
 */
function activate(context) {
	const selector = { language: 'painperdu' };
	context.subscriptions.push(vscode.languages.registerDocumentSemanticTokensProvider(selector, new DocumentSemanticTokensProvider(), legend));
	context.subscriptions.push(vscode.languages.registerCompletionItemProvider(selector, new GoCompletionItemProviderLabels(), '.', '*', '&'));
	context.subscriptions.push(vscode.languages.registerCompletionItemProvider(selector, new GoCompletionItemProviderReference(), '#', '@', '$', '>', '<', '+', '-', '?', '!'));
}

function deactivate() { }

module.exports = {
	activate,
	deactivate
}
