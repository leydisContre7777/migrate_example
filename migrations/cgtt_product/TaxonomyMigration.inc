<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 30/11/17
 * Time: 14:10
 */

class TaxonomyMigration extends CgttBaseMigration {
	public function __construct($arguments, $parent, $vocabulary_name) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('trove_cat')
			->fields('trove_cat', array('trove_cat_id',
				'fullname',
				'description'
			));
		$query->condition('parent', $parent)
			->orderBy('fullname', 'ASC');




		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));

		// Set up our destination - terms in the migrate_example_beer_styles
		// vocabulary (note that we pass the machine name of the vocabulary).
		$this->destination =
			new MigrateDestinationTerm($vocabulary_name);

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'trove_cat_id' => array('type' => 'int',
					'length' => 255,
					'not null' => TRUE,
					'description' => 'Subject ID',
				)
			),
			MigrateDestinationTerm::getKeySchema()
		);

		$this->addFieldMapping('name', 'fullname');
		$this->addFieldMapping('description', 'description');
	}

}
