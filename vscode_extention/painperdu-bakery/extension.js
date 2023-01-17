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
			//console.log("Token:\n\tline: " + (token.get_line() - 1)+ "\n\tcolumn: " + (token.get_start_column() - 1) + "\n\tlength: " + token.get_length() + "\n\ttype: " + token.get_type() + "\n")
		}
		return builder.build();
	}
}

/**
 * @param {vscode.ExtensionContext} context
 */
function activate(context) {
	console.log('Congratulations, your extension "painperdurecipe" is now active!');
	context.subscriptions.push(vscode.languages.registerDocumentSemanticTokensProvider({ language: 'painperdu' }, new DocumentSemanticTokensProvider(), legend));
}

function deactivate() {}

module.exports = {
	activate,
	deactivate
}