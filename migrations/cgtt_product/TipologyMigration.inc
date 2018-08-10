<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 30/11/17
 * Time: 14:10
 */

class TipologyMigration extends CgttBaseMigration {
	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('doc_groups');
		$query->addExpression('DISTINCT ON (groupname) groupname', 'groupname');
		$query->addExpression('doc_group', 'doc_group');
		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));
		$this->destination = new MigrateDestinationTerm('tipolog_a_producto');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'doc_group' => array('type' => 'int',
					'length' => 255,
					'not null' => TRUE,
					'description' => 'Doc ID',
				)
			),
			MigrateDestinationTerm::getKeySchema()
		);

		$this->addFieldMapping('name', 'groupname');

		$this->addUnmigratedDestinations(array(
			'description'
		));
	}

}
